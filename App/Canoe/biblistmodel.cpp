#include "biblistmodel.h"
#include <QDebug>
#include <QSettings>
#include <global.h>
#include <algorithm>
#include "nickname.h"
#include <QTimer>
#include <QFile>
#include "bib.h"
#include "csvparser.h"
#include "trapsdb.h"
#include "penaltyviewmodel.h"

#define COLUMN_COUNT 31

BibListModel::BibListModel(TRAPSDB *database, PenaltyViewModel *penaltyViewModel) : SpreadSheet (COLUMN_COUNT, QStringList() << "Dossard" << "Catégorie" << "Horaire" << "Départ" << "Arrivée" << "Chrono"
                                                              << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12"
                                                              << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23"
                                                              << "24" << "25",
                                            QList<double>() << 4.5 << 5 << 5.5 << 6.5 << 6.5 << 6.5
                                                            << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3
                                                            << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3, 6),
    _db(database),
    _penaltyView(penaltyViewModel)
{

    Q_ASSERT(_db);
    Q_ASSERT(_penaltyView);
    //_db->dump(DB_PENALTIES);
    for (int i=0; i<COLUMN_COUNT; i++) {
        setReadOnlyForColumn(i);
        setTextAlignmentForColumn(i, Qt::AlignHCenter);
        setSortEnabledForColumn(i, false);
        setFontFamilyForColumn(i, "monospace");
        if (i>5) setActionEnabledForColumn(i);
    }
    setTextAlignmentForColumn(3, Qt::AlignRight);
    setTextAlignmentForColumn(4, Qt::AlignRight);
    setTextAlignmentForColumn(5, Qt::AlignRight);
    setBgColorForColumn(0, MATERIAL_SHADE_50);
    setBgColorForColumn(1, MATERIAL_SHADE_50);
    setBgColorForColumn(2, MATERIAL_SHADE_50);
    setFontFamilyForColumn(1, "");

    bool showChrono = chronoVisible();
    setColumnListVisible(QList<int>() << 3 << 4 << 5, showChrono);

    reload();
    QSettings settings;
    _scheduling = settings.value("scheduling", 0).toInt();
    _penaltyDepth = settings.value("penaltyDepth", 1).toInt();

}

BibListModel::~BibListModel() { }

int BibListModel::bibCount() const {
    return _bibList.count();
}

// first of pair is pointer to bib, second is the index of the penalty depth. Index is always 0 when teamMode==false, 0..2 when teamMode==true
QPair<Bib*, int> BibListModel::bibAtRowIndex(int rowIndex) const {
    if (rowIndex<0 || rowIndex>=_bibList.count()*_penaltyDepth) return qMakePair(nullptr,0);
    int index = rowIndex / _penaltyDepth;
    int shift = 0;
    if (_penaltyDepth==MAX_PENALTY_DEPTH) shift = rowIndex%3;
    return qMakePair(_bibList.at(index), shift);
}

Bib* BibListModel::bibWithNumber(int number) const {
    foreach (Bib* bib, _bibList) {
        if (bib->number()==number) return bib;
    }
    return 0;
}


int BibListModel::bibIndex(int bibnumber) const {
    for (int index=0; index<_bibList.count(); index++) {
        Bib* bib = _bibList.at(index);
        if (bib->number()==bibnumber) return index;
    }
    return -1;
}

bool BibListModel::chronoVisible() const {
    QSettings settings;
    return settings.value("chronoVisible", false).toBool();
}

bool BibListModel::teamMode() const {
    QSettings settings;
    return settings.value("teamMode", false).toBool();

}


void BibListModel::requestAction(int rowIndex, int columnIndex) {
    qDebug() << "Request action for row " << rowIndex << " columnIndex " << columnIndex;
    if (columnIndex<6 || columnIndex>31) return;
    auto bibPair = bibAtRowIndex(rowIndex);
    Bib* bib = bibPair.first;
    if (bib==nullptr) {
        qCritical() << "Cannot find bib at rowIndex " << rowIndex;
        return;
    }
    int depth = bibPair.second;
    int gateId = columnIndex-5;
    int value = bib->valueAtGate(gateId, depth);
    if (value!=2) {
        qInfo() << "Penalty is not 2. Nothing to do.";
        return;
    }

    PenaltySet* penaltySet = bib->penaltySetAtGate(gateId);
    if (penaltySet==nullptr) {
        qCritical() << "No penalty found at gate " << gateId << " for bib " << bib->number();
        return;
    }

    Penalty penalty = penaltySet->selectedPenalty();
    if (penalty.value()<0) {
        qInfo() << "No penalty found or no selected sender. Cannot choose which penalty to display.";
        return;
    }


    QVariantMap map;
    map.insert(PenaltyViewModel::title, QString("Dossard %1 - Porte %2").arg(bib->numberStr()).arg(gateId));
    map.insert(PenaltyViewModel::sender, penalty.senderId());
    map.insert(PenaltyViewModel::timestamp, penalty.timestamp());
    map.insert(PenaltyViewModel::xspot1, penalty.spot1().first);
    map.insert(PenaltyViewModel::yspot1, penalty.spot1().second);
    map.insert(PenaltyViewModel::xspot2, penalty.spot2().first);
    map.insert(PenaltyViewModel::yspot2, penalty.spot2().second);
    map.insert(PenaltyViewModel::teammate, penalty.teammate());
    _penaltyView->set(map);

    emit _penaltyView->openPenaltyDialog();
}

QStringList BibListModel::contextMenuList(int rowIndex, int columnIndex) const {

    QStringList list;
    if (columnIndex<6) {
        if (selectedRowSet().isEmpty()) {
            auto bib = bibAtRowIndex(rowIndex);
            if (bib.first==nullptr) return list;
            if (bib.first->locked()) list << QString("Déverrouiller le dossard %0").arg(bib.first->displayId());
            else list << QString("Verrouiller le dossard %0").arg(bib.first->displayId());
            list << QString("Renvoyer le dossard %0 à CompetFFCK").arg(bib.first->displayId());
        }
        else {
            list << "Verrouiller les dossards sélectionnés";
            list << "Déverrouiller les dossards sélectionnés";
            list << "Renvoyer les dossards sélectionnés à CompetFFCK";
        }
    } else {
        auto bibPair = bibAtRowIndex(rowIndex);
        Bib* bib = bibPair.first;
        int depth = bibPair.second;
        if (bib==nullptr) return list;
        int gateId = columnIndex-5;
        int penalty = bib->valueAtGate(gateId, depth);
        if (penalty!=2) {
            qDebug() << "Penalty is not 2. Nothing to do.";
            return list;
        }
        PenaltySet* penaltySet = bibPair.first->penaltySetAtGate(gateId, depth);
        if (penaltySet!=nullptr) {
            list = penaltySet->senderList();
        }

    }
    return list;
}

void BibListModel::contextMenuAction(int rowIndex, int columnIndex, int menuIndex) {

    if (selectedRowSet().isEmpty()) {

        switch (menuIndex) {
            case 0: {
                auto bib = bibAtRowIndex(rowIndex);
                if (bib.first==nullptr) return;
//                QSet<int>
//                if (bib->locked()) unlock()
                return;
            }
            case 1: {


            }
        }
    }
    else {



    }

}

void BibListModel::setChronoVisible(bool visible) {
    // postpone the process to allow the UI to check/uncheck the box first

    QTimer::singleShot(100, [this, visible]() {
        QSettings settings;
        settings.setValue("chronoVisible", visible);
        setColumnListVisible(QList<int>() << 3 << 4 << 5, visible);
    });

}

void BibListModel::setTeamMode(bool modeOn) {

    QTimer::singleShot(100, [this, modeOn]() {
        QSettings settings;
        settings.setValue("teamMode", modeOn);
        beginResetModel();
        if (modeOn) _penaltyDepth = MAX_PENALTY_DEPTH;
        else _penaltyDepth = 1;
        endResetModel();
    });

}

QString BibListModel::text(int rowIndex, int columnIndex) const {

    auto bib = bibAtRowIndex(rowIndex);
    if (bib.first==nullptr) return QString();

    if (columnIndex<6 && bib.second==0) {
        switch (columnIndex) {
            case 0 : return bib.first->displayId();
            case 1 : return bib.first->categ();
            case 2 : return bib.first->schedule();
            case 3 : return bib.first->startTimeStr();
            case 4 : return bib.first->finishTimeStr();
            case 5 : return bib.first->runningTimeStr();
        }
    }

    if (columnIndex<31) {
        QString penaltyStr = bib.first->stringValueAtGate(columnIndex-5, bib.second);
        if (penaltyStr=="0") penaltyStr = "";
        return penaltyStr;
    }

    return QString();
}

QString BibListModel::bgColor(int rowIndex, int columnIndex) const {

    if (columnIndex>-1 && columnIndex<3 && selectedRowSet().contains(rowIndex)) return MATERIAL_SHADE_700;

    if (columnIndex<6) return QString();
    auto bib = bibAtRowIndex(rowIndex);
    if (bib.first==nullptr) return QString();

    switch (bib.first->valueAtGate(columnIndex-5, bib.second)) {
        case PENALTY_0  : return "lightgreen";
        case PENALTY_2  : return "yellow";
        case PENALTY_50 : return "tomato";
        case PENALTY_CONFLICT : return "black";
    }

    return QString();
}

QString BibListModel::textColor(int rowIndex, int columnIndex) const {
    if (columnIndex<6) return QString();
    auto bib = bibAtRowIndex(rowIndex);
    if (bib.first==nullptr) return QString();

    switch (bib.first->valueAtGate(columnIndex-5, bib.second)) {
        case 100 : return "white";
    }

    return QString();
}

int BibListModel::rowCount() const {
    return _bibList.count()*_penaltyDepth;
}

void BibListModel::reload() {
    qDebug() << "Reloading bib list from database";
    beginResetModel();
    qDeleteAll(_bibList);
    _bibList.clear();

    auto bibs = _db->map(DB_BIB);
    auto locks = _db->map(DB_LOCKS);
    auto times = _db->map(DB_TIMES);
    auto penalties = _db->map(DB_PENALTIES);
    foreach (QString bibNumber, bibs.keys()) {
        Bib* bib = new Bib(bibs.value(bibNumber));
        bib->setFinishTime((qint64)times.value(bibNumber).value("finishTime").toDouble());
        bib->setStartTime((qint64)times.value(bibNumber).value("startTime").toDouble());
        bib->initWithPenaltyList(penalties.value(bibNumber));
        bib->setLocked(locks.value(bibNumber).value("locked").toBool(false));
        _bibList.append(bib);
    }
    endResetModel();
    qDebug() << "Bib count: " << _bibList.count();
    orderBibList();
    emit bibCountChanged(_bibList.count());
}


void BibListModel::loadCSV(const QString& filename, bool reset) {

    if (filename.isEmpty()) {
        qWarning() << "No file selected. Abort.";
        return;
    }

    QString path = QUrl(filename).toLocalFile();
    qDebug() << "Loading bib list from: " << path;
    QFile file(path);
    // use QFile::exists("blabla"); !
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Cannot open file " << path;
        emit error("Chargement fichier CSV", QString("Impossible d'ouvrir le fichier\n%0").arg(path));
        return;
    }

    QTextStream stream(&file);
    QString string;

    QHash<QString,QJsonObject> bibs;
    QHash<QString,QJsonObject> locks;
    QHash<QString,QJsonObject> times;
    QHash<QString,QJsonObject> penalties;
    int row = 0;
    emit toast("Chargement des dossards...", 3000);
    bool errorRow = 0;
    while (true) {
        row++;
        string = stream.readLine().trimmed();
        if (string.isNull()) break;
        if (string.isEmpty()) continue;
        qDebug() << string;
        QStringList tabComma = string.split(',');
        QStringList tabSemiColumn = string.split(';');
        QStringList tab = tabComma.count()>tabSemiColumn.count()?tabComma:tabSemiColumn; // take the delimiter that gives the max number of elements
        // bib number
        bool ok;
        int bibnumber = tab[0].toInt(&ok);
        if (!ok || bibnumber<1) {
            qWarning() << "Cannot convert the bib number read in this line: " << string;
            errorRow = row;
            break;
        }
        QString categ = "-";
        if (tab.count()>1) categ = tab[1];
        QString schedule = "-";
        if (tab.count()>2) schedule = tab[2];

        Bib bib(bibnumber);
        bib.setCateg(categ);
        bib.setEntry(row);
        bib.setSchedule(schedule);
        bibs.insert(bib.numberStr(), bib.jsonParam());
        locks.insert(bib.numberStr(), bib.jsonLock());
        times.insert(bib.numberStr(), bib.jsonTime());
        penalties.insert(bib.numberStr(), bib.jsonPenalty());

    }

    if (errorRow>0) {
        emit error("Erreur de lecture fichier", QString("Impossible de décoder la ligne %0:\n%1").arg(errorRow).arg(string));
        return;
    }

    if (reset) {
        _db->clear();
        emit toast("Dossards actuels effacés", 3000);
    }
    // save biblist in DB
    if (bibs.count()>0) {
        _db->setValueMap(DB_BIB, bibs);
        _db->setValueMap(DB_LOCKS, locks);
        _db->setValueMap(DB_TIMES, times);
        _db->setValueMap(DB_PENALTIES, penalties);

    }

    reload();
    emit toast(QString("Liste de %0 dossards chargée").arg(bibs.count()), 4500);

}

void BibListModel::clearPenalties() {
    beginResetModel();
    foreach (Bib* bib, _bibList) {
        bib->clearPenalties();
    }
    _db->drop(DB_PENALTIES);
    endResetModel();
    emit toast("Penalités effacées", 3000);
}

void BibListModel::clearChronos() {
    beginResetModel();
    foreach (Bib* bib, _bibList) {
        bib->clearChronos();
    }
    _db->drop(DB_TIMES);
    endResetModel();
    emit toast("Chrono effacés", 3000);
}

void BibListModel::processIncomingPenaltyList(QList<Penalty> penaltyList) {

    foreach (Penalty penalty, penaltyList) {
        qDebug() << "Processing penalty for bib " << penalty.bib() << " gate " << penalty.gateId();
        Bib* bib = bibWithNumber(penalty.bib().first);
        if (bib==0) {
            qWarning() << "Cannot find bib " << penalty.bib().first;
            return;
        }
        if (bib->setPenalty(penalty)) {
            int val = bib->valueAtGate(penalty.gateId());
            if (val==PENALTY_0 || val==PENALTY_2 || val==PENALTY_50) {
                qDebug() << "Biblist emiting send penalty for gate " << penalty.gateId();
                emit penaltyUpdate(penalty.bib(), penalty.gateId(), val);
            }
            // update db
            _db->setValue(DB_PENALTIES, bib->numberStr(), bib->jsonPenalty());

            // update view

            int index = bibIndex(penalty.bib().first);
            int row = index*_penaltyDepth + penalty.bib().second;
            rowUpdated(row);
            qDebug() << "Updating row " << row;
        }

    }
}

void BibListModel::processIncomingPenalty(int senderId, int bibnumber, QHash<int, int> penaltyList) {
    qDebug() << "Biblist processing incoming penalties for bib " << bibnumber;
    Bib* bib = bibWithNumber(bibnumber);
    if (bib==0) {
        qWarning() << QString("Cannot find bib id %0 in the list. Ignore.").arg(bibnumber);
        return;
    }
    int bibRow = bibIndex(bibnumber);
    foreach (int gateId, penaltyList.keys()) {
        int penaltyValue = penaltyList.value(gateId);
        Penalty penalty(qMakePair(bib->number(),0), gateId, penaltyValue);
        QString nickName = NickName::name256.value(senderId%256);
        qDebug() << "Penalty received from " << nickName;
        penalty.setSender("0", nickName);
        if (bib->setPenalty(penalty)) {
            int val = bib->valueAtGate(gateId);
            if (val==PENALTY_0 || val==PENALTY_2 || val==PENALTY_50) {
                qDebug() << "Biblist emiting send penalty for gate " << gateId;
                emit penaltyUpdate(qMakePair(bibnumber,0), gateId, val);
            }
        }
        else {
            qDebug() << "Bib is locked, cannot set penalty. abort.";
            toast(QString("Dossard %0 est vérrouillé. Pénalité porte %1 ignorée.").arg(bib->numberStr()).arg(gateId), 3000);
        }
    }
    _db->setValue(DB_PENALTIES, bib->numberStr(), bib->jsonPenalty());
    // Notify penalties for this bib has changed
    rowUpdated(bibRow);
}

void BibListModel::processIncomingStartTime(int bibnumber, qint64 startTime) {
    qDebug() << "Biblist processing incoming start time for bib " << bibnumber;
    Bib* bib = bibWithNumber(bibnumber);
    if (bib==0) {
        qWarning() << QString("Cannot find bib id %0 in the list. Ignore.").arg(bibnumber);
        return;
    }
    if (bib->setStartTime(startTime)) {
        _db->setValue(DB_TIMES, bib->numberStr(), bib->jsonTime());
        int runningTime = bib->runningTime();
        qDebug() << "RUNNING TIME: "<< runningTime;
        if (runningTime>0) emit chronoReceived(bibnumber, runningTime);
        int bibRow = bibIndex(bib->number());
        rowUpdated(bibRow);
    }
    else {
        qDebug() << "Bib is locked, cannot set start time. abort.";
        toast(QString("Dossard %0 est vérrouillé. Heure de départ ignorée.").arg(bib->numberStr()), 3000);
    }

}

void BibListModel::processIncomingFinishTime(int bibnumber, qint64 finishTime) {
    qDebug() << "Biblist processing incoming finish time for bib " << bibnumber;
    Bib* bib = bibWithNumber(bibnumber);
    if (bib==nullptr) {
        qWarning() << QString("Cannot find bib id %0 in the list. Ignore.").arg(bibnumber);
        return;
    }
    if (bib->setFinishTime(finishTime)) {
        _db->setValue(DB_TIMES, bib->numberStr(), bib->jsonTime());
        int runningTime = bib->runningTime();
        if (runningTime>0) emit chronoReceived(bibnumber, runningTime);
        int bibRow = bibIndex(bib->number());
        rowUpdated(bibRow);
    }
    else {
        qDebug() << "Bib is locked, cannot set finish time. abort.";
        toast(QString("Dossard %0 est vérrouillé. Heure d'arrivée ignorée.").arg(bib->numberStr()), 3000);
    }

}

void BibListModel::processIncomingLapTime(int bibnumber, int lap, qint64 time){

}

void BibListModel::selectPenalty(int bibIndex, int gateIndex) {

    qDebug() << QString("You just selected bib index %0 at gate index %1").arg(bibIndex).arg(gateIndex);

}

void BibListModel::setScheduling(int criteria) {


    qDebug() << "Scheduling is now " << criteria;
    _scheduling = criteria;
    QSettings settings;
    settings.setValue("scheduling", _scheduling);

    orderBibList();

}

void BibListModel::setLock(bool lock, QSet<int> rowIndexSet) {
    qDebug() << "Locking rows: " << lock;
    QHash<QString,QJsonObject> valueMap;
    int i=0;
    foreach (int row, rowIndexSet) {
        auto bib = bibAtRowIndex(row);
        if (bib.first!=nullptr) {
            bib.first->setLocked(lock);
            valueMap.insert(bib.first->numberStr(), bib.first->jsonLock());
            i++;
        }
    }
    _db->setValueMap(DB_LOCKS, valueMap);

    QString message;
    if (lock) message = (i<2)?QString("1 dossard vérrouillé"):QString("%0 dossards vérrouillés").arg(i);
    else message = (i<2)?QString("1 dossard déverrouillé"):QString("%0 dossards déverrouillés").arg(i);

    emit toast(message, 2000);
}

void BibListModel::setLock(bool lock, int rowIndex) {
    setLock(lock, QSet<int>() << rowIndex);
}


void BibListModel::forwardBib(QSet<int> rowIndexSet) {
//    qDebug() << "Forwarding bibs";
//    int i=0;
//    foreach (int row, rowIndexSet) {
//        Bib* bib = bibAtIndex(row);
//        if (bib!=0) {
//            if (bib->runningTime()>0) emit chronoReceived(bib->number(), bib->runningTime());
//            QHash<int, Penalty> penaltyList = bib->penaltyList();
//            foreach (int gateId, penaltyList.keys()) {
//                int val = penaltyList.value(gateId).value();
//                if (val == PENALTY_0 || val == PENALTY_2 || val == PENALTY_50) {
//                    emit penaltyUpdate(bib->idNumber(), gateId, val);
//                }
//            }
//            i++;
//        }
//    }

//    QString message = (i<2)?QString("1 dossard renvoyé"):QString("%0 dossards renvoyés").arg(i);
//    emit toast(message, 2000);

}

void BibListModel::forwardBib(int rowIndex) {
    forwardBib(QSet<int>() << rowIndex);

}

QJsonArray BibListModel::jsonArray() const {
    QJsonArray array;
    foreach (Bib* bib, _bibList) {
        array << bib->jsonParam();
    }
    return array;
}

QList<int> BibListModel::bibNumberList() const {
    QList<int> list;
    foreach (Bib* bib, _bibList) {
        int number = bib->number();
        if (number>0) list << number;
    }
    return list;
}

void BibListModel::orderBibList() {
    beginResetModel();
    switch (_scheduling) {
        case 0 : {
            std::sort(_bibList.begin(), _bibList.end(), [](Bib* a, Bib* b) { return a->number()<b->number(); });
            emit toast("Liste ordonnée selon les numéros de dossard croissants", 3000);
            break;
        }
        case 1 : {
            std::sort(_bibList.begin(), _bibList.end(), [](Bib* a, Bib* b) { return a->schedule()<b->schedule(); });
            emit toast("Liste ordonnée selon les heures de départ", 3000);
            break;
        }
        case 2 : {
            std::sort(_bibList.begin(), _bibList.end(), [](Bib* a, Bib* b) { return a->entry()<b->entry(); });
            emit toast("Liste ordonnée selon le rang dans le fichier d'origine", 3000);
            break;
        }
    }
    endResetModel();
}

