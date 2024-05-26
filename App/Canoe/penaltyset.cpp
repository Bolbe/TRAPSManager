#include "penaltyset.h"
#include <QJsonObject>
#include <QJsonArray>

PenaltySet::PenaltySet():
    _gateId(0)
{

}

PenaltySet::PenaltySet(int gateId) {
    _gateId = gateId;
}

PenaltySet::PenaltySet(const PenaltySet &other) {
    _selectedSender = other._selectedSender;
    _set = other._set;
    _gateId = other._gateId;
}

PenaltySet::PenaltySet(const QJsonObject &jsonObj) {
    _selectedSender = jsonObj.value("selectedSender").toString();
    _gateId = jsonObj.value("gateId").toInt();
    foreach (QJsonValue value, jsonObj.value("penaltySet").toArray()) {
        Penalty penalty(value.toObject());
        _set.insert(penalty.senderId(), penalty);
    }

}

void PenaltySet::insert(const Penalty &penalty) {
    _set.insert(penalty.senderId(), penalty);
    _selectedSender = "";
}

int PenaltySet::value() const {
    if (_set.isEmpty()) return PENALTY_NOTSET;
    if (_selectedSender.isEmpty()) {
        int value = PENALTY_NOTSET;
        foreach (Penalty penalty, _set.values()) {
            if (value==PENALTY_NOTSET) value=penalty.value();
            else if (value!=penalty.value()) return PENALTY_CONFLICT;
        }
        return value;
    }
    return _set.value(_selectedSender).value();
}


void PenaltySet::setSelectedSender(const QString &sender) {
    if (!_set.contains(sender)) return;
    _selectedSender = sender;
}

Penalty PenaltySet::selectedPenalty() const {
    if (_selectedSender.isEmpty() && _set.count()==1) {
        return _set.values().constFirst();
    }
    return _set.value(_selectedSender);
}

QJsonObject PenaltySet::jsonObject() const {
    QJsonObject json;
    if (!_selectedSender.isEmpty()) json.insert("selectedSender", _selectedSender);
    json.insert("gateId", _gateId);
    QJsonArray array;
    foreach (Penalty penalty, _set.values()) {
        array.append(penalty.jsonObject());
    }
    json.insert("penaltySet", array);

    return json;
}
