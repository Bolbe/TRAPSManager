#include "loadercsv.h"
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <Canoe/bib.h>
#include "trapsdb.h"

LoaderCSV::LoaderCSV(Database* database) :
    BibLoader(database)
{

}

void LoaderCSV::processLoad(const QString &fileName, bool reset)
{

}

void LoaderCSV::process()
{
    if (_fileName.isEmpty()) {
        qWarning() << "No file selected. Abort.";
        return;
    }

    QString path = QUrl(_fileName).toLocalFile();
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

    if (_reset) {
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

    emit toast(QString("Liste de %0 dossards chargée").arg(bibs.count()), 4500);
    emit loaded();
}

