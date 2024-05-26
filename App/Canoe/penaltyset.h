#ifndef PENALTYSET_H
#define PENALTYSET_H

#include <QHash>
#include "penalty.h"

class PenaltySet
{
public:

    PenaltySet();
    PenaltySet(int gateId);
    PenaltySet(const QJsonObject& jsonObj);
    PenaltySet(const PenaltySet& other);

    void insert(const Penalty& penalty);
    int value() const;
    QString selectedSender() const { return _selectedSender; }
    void setSelectedSender(const QString& sender);
    int gateId() const { return _gateId; }
    QStringList senderList() const { return _set.keys(); }
    Penalty selectedPenalty() const;
    QJsonObject jsonObject() const;


private :

    int _gateId;
    QString _selectedSender;
    QHash<QString, Penalty> _set;


};

#endif // PENALTYSET_H
