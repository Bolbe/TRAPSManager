#include "bib.h"
#include "penalty.h"
#include <QHash>
#include <QJsonArray>
#include <QJsonValue>
#include <QDateTime>
#include <QDebug>


Bib::Bib(int number):
    _number(number),
    _startTime(0),
    _finishTime(0),
    _locked(false),
    _timestamp(0)
{
    init();
}


Bib::Bib(const QJsonObject &json) :
    _number(0),
    _startTime(0),
    _finishTime(0),
    _locked(false),
    _timestamp(0)
{

    _number = json.value("number").toInt();
    _categ = json.value("categ").toString("?");
    _entry = json.value("entry").toInt(0);
    _schedule = json.value("schedule").toString("?");
    init();

}


void Bib::init() {
    _displayId = QString::number(_number).rightJustified(3,'0');
    for (int i=0; i<MAX_PENALTY_DEPTH; i++) {
        _penalties.append(new PenaltyList());
    }
}

void Bib::setEntry(int entry) {
    _entry = entry;
}

bool Bib::setStartTime(qint64 startTime) {
    if (locked()) return false;
    _startTime = startTime;
    return true;
}

QString Bib::startTimeStr() const {
    if (_startTime<1) return "-";
    QDateTime time = QDateTime::fromMSecsSinceEpoch(_startTime);
    return time.toString("HH:mm:ss.zzz");
}

bool Bib::setFinishTime(qint64 finishTime) {
    if (locked()) return false;
    _finishTime = finishTime;
    return true;
}

QString Bib::finishTimeStr() const {
    if (_finishTime<1) return "-";
    QDateTime time = QDateTime::fromMSecsSinceEpoch(_finishTime);
    return time.toString("HH:mm:ss.zzz");
}

qint64 Bib::runningTime() const {
    if (_finishTime<1 || _startTime<1) return 0;
    qint64 value = _finishTime - _startTime;
    if (value<1) return 0;
    return value;
}

QString Bib::runningTimeStr() const {
    qint64 value = runningTime();
    if (value<1) return "-";
    QTime time = QTime(0,0).addMSecs(value);
    return time.toString("mm:ss.zzz");
}

void Bib::setSchedule(const QString &schedule) {
    _schedule = schedule;
}

void Bib::setCateg(const QString &categ) {
    _categ = categ;
}

void Bib::setLocked(bool locked) {
    _locked = locked;
}

QList<QHash<int, int>> Bib::penaltyValueList() const {
    QList<QHash<int, int>> list;

    foreach (PenaltyList* penaltyList, _penalties) {
        QHash<int, int> subList;
        foreach (PenaltySet* penaltySet, penaltyList->values()) {
            subList.insert(penaltySet->gateId(), penaltySet->value());
        }
        list.append(subList);
    }
    return list;
}

int Bib::valueAtGate(int gateId, int depth) const {
    PenaltySet* penaltySet = penaltySetAtGate(gateId, depth);
    if (penaltySet==0) return PENALTY_NOTSET;
    return penaltySet->value();

}

QString Bib::stringValueAtGate(int gateId, int depth) const {
    int val = valueAtGate(gateId, depth);
    switch (val) {
        case PENALTY_0: return "0";
        case PENALTY_2: return "2";
        case PENALTY_50: return "50";
        case PENALTY_CONFLICT: return "?";
    }
    return "";
}

PenaltySet* Bib::penaltySetAtGate(int gateId, int depth) const {
    if (gateId<1 || gateId>GATE_MAX_COUNT || depth<0 || depth>=_penalties.size()) return nullptr;
    return _penalties[depth]->value(gateId);
}

bool Bib::setPenalty(const Penalty &penalty) {
    int depth = penalty.bib().second;
    if (locked() || depth<0 || depth>=_penalties.size()) return false;
    int gateId = penalty.gateId();
    if (gateId>0 && gateId<=GATE_MAX_COUNT) {
        PenaltyList* penaltyList = _penalties.at(penalty.bib().second);
        PenaltySet* penaltySet = penaltyList->value(gateId);
        if (penaltySet==0) {
            penaltySet = new PenaltySet(gateId);
            penaltyList->insert(gateId, penaltySet);
        }
        penaltySet->insert(penalty);
        return true;
    }
    return false;
}

bool Bib::setPenalty(const QList<Penalty> &penaltyList) {
    if (locked()) return false;
    foreach (Penalty penalty, penaltyList) {
        setPenalty(penalty);
    }
    return true;
}

void Bib::clearPenalties() {
    foreach (PenaltyList* list, _penalties) {
        qDeleteAll(*list);
        list->clear();
    }

}

void Bib::clearChronos() {
    setStartTime(0);
    setFinishTime(0);
}

QString Bib::toString() {
    return QString("%1|%2|%3").arg(_number).arg(_categ).arg(_schedule);
}


QJsonObject Bib::jsonParam() const {
    QJsonObject obj;
    obj.insert("number", _number);
    obj.insert("categ", _categ);
    obj.insert("schedule", _schedule);
    obj.insert("entry", _entry);
    return obj;
}

QJsonObject Bib::jsonLock() const {
    QJsonObject obj;
    obj.insert("locked", _locked);
    return obj;
}

QJsonObject Bib::jsonTime() const {
    QJsonObject obj;
    obj.insert("startTime", _startTime);
    obj.insert("finishTime", _finishTime);
    return obj;
}

QJsonObject Bib::jsonPenalty() const {
    QJsonObject obj;
    QJsonArray array;

    foreach (PenaltyList* list, _penalties) {
        QJsonArray subList;
        foreach (PenaltySet* penaltySet, list->values()) {
            subList.append(penaltySet->jsonObject());
        }
        array.append(subList);
    }
    obj.insert("penaltyList", array);
    return obj;
}

void Bib::initWithPenaltyList(const QJsonObject &jsonObj) {
    clearPenalties();
    QJsonArray array = jsonObj.value("penaltyList").toArray();

    int index = 0;
    foreach (QJsonValue jsonArray, array) {
        if (index>=MAX_PENALTY_DEPTH) break;
        PenaltyList* penaltyList = _penalties.at(index);
        foreach (QJsonValue jsonValue, jsonArray.toArray()) {
            PenaltySet* penaltySet = new PenaltySet(jsonValue.toObject());
            penaltyList->insert(penaltySet->gateId(), penaltySet);
        }
        index++;
    }
}
