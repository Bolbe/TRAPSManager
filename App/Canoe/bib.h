#ifndef BIB_H
#define BIB_H

#include "penalty.h"
#include "penaltyset.h"
#include <QList>
#include <QHash>

#define GATE_MAX_COUNT 25
#define MAX_PENALTY_DEPTH 3

class Bib {

public:

    typedef QHash<int, PenaltySet*> PenaltyList;

    explicit Bib(int number);

    // used when loading from db
    explicit Bib(const QJsonObject& json);
    void initWithPenaltyList(const QJsonObject &jsonObj);

    int entry() const { return _entry; }
    void setEntry(int entry);

    qint64 startTime() const { return _startTime; }
    bool setStartTime(qint64 startTime);
    QString startTimeStr() const;

    qint64 finishTime() const { return _finishTime; }
    bool setFinishTime(qint64 finishTime);
    QString finishTimeStr() const;

    qint64 runningTime() const;
    QString runningTimeStr() const;

    int timestamp() const { return _timestamp; }
    void setTimestamp(int timestamp);

    QString schedule() const { return _schedule; }
    void setSchedule(const QString &schedule);

    QString categ() const { return _categ; }
    void setCateg(const QString &categ);

    bool locked() const { return _locked; }
    void setLocked(bool locked);

    int number() const { return _number; }
    QString numberStr() const { return QString::number(_number); }

    QJsonObject jsonParam() const;
    QJsonObject jsonLock() const;
    QJsonObject jsonTime() const;
    QJsonObject jsonPenalty() const;

    QList<QHash<int, int> > penaltyValueList() const;
    int valueAtGate(int gateId, int depth = 0) const;
    QString stringValueAtGate(int gateId, int depth = 0) const;
    PenaltySet* penaltySetAtGate(int gateId, int depth = 0) const;
    bool setPenalty(const Penalty& penalty); // returns false if bib is locked
    bool setPenalty(const QList<Penalty>& penaltyList); // returns false if bib is locked
    void clearPenalties();
    void clearChronos();

    QString toString();
    QString displayId() const { return _displayId; }

private:

    int _number;
    QString _displayId;
    int _entry;
    qint64 _startTime;
    qint64 _finishTime;
    QList<PenaltyList*> _penalties;
    QString _schedule;
    QString _categ;
    bool _locked;
    int _timestamp;


    void init();
};

#endif // BIB_H
