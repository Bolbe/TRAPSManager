#include "penalty.h"
#include <QDateTime>
#include <QJsonArray>

/*

{
    "bib": [2,0],
    "gateId": 1,
    "value": 2,
    "canvas": {
        "spot1": [0.25, 0.75],
        "spot2": [0.3, 0.4],
        "teammate": true
    },
    "timestamp": "1628676685123",
    "sender": {
        "ip": "192.168.1.12",
        "name": "NABUKO"
    }

}


*/


Penalty::Penalty() :
    _bib(qMakePair(0,0)),
    _value(-1),
    _gateId(-1)
{


}

Penalty::Penalty(const Penalty &other) {

    _bib = other._bib;
    _sender = other._sender;
    _senderId = other._senderId;
    _gateId = other._gateId;
    _value = other._value;
    _canvas = other._canvas;
    _timestamp = other._timestamp;
    _teammate = other._teammate;
    _spot1 = other._spot1;
    _spot2 = other._spot2;


}


Penalty::Penalty(QPair<int, int> bib, int gateId, int value) :
    _bib(bib),
    _value(value),
    _canvas(false),
    _gateId(gateId)
{
    _timestamp = QDateTime::currentMSecsSinceEpoch();

}

Penalty::Penalty(const QJsonObject &json) {

    QJsonArray jsonBib = json.value("bib").toArray();
    _bib = qMakePair(jsonBib.at(0).toInt(), jsonBib.at(1).toInt());
    _gateId = json.value("gateId").toInt();
    _value = json.value("value").toInt();
    _timestamp = json.value("timestamp").toString().toLongLong();
    QJsonObject canvas = json.value("canvas").toObject();
    if (canvas.isEmpty()) {
        _canvas = false;
    }
    else {
        _canvas = true;
        _teammate = canvas.value("teammate").toBool();
        QJsonArray jsonSpot1 = canvas.value("spot1").toArray();
        _spot1 = qMakePair(jsonSpot1.at(0).toDouble(), jsonSpot1.at(1).toDouble());
        QJsonArray jsonSpot2 = canvas.value("spot2").toArray();
        _spot2 = qMakePair(jsonSpot2.at(0).toDouble(), jsonSpot2.at(1).toDouble());

    }
    QJsonObject sender = json.value("sender").toObject();
    if (!sender.isEmpty()) {
        _sender.ip = sender.value("ip").toString();
        _sender.name = sender.value("name").toString();
        rebuildSenderId();
    }

}


QString Penalty::stringValue() const {
    switch (_value) {
        case PENALTY_0: return "0";
        case PENALTY_2: return "2";
        case PENALTY_50: return "50";
        case PENALTY_CONFLICT: return "?";
    }
    return "";
}

void Penalty::setSpot1(double x, double y) {
    _spot1.first = x;
    _spot1.second = y;
}

void Penalty::setSpot2(double x, double y) {
    _spot2.first = x;
    _spot2.second = y;
}


void Penalty::setTeammate(bool teammate) {
    _teammate = teammate;
}

void Penalty::setCanvas(bool canvas) {
    _canvas = canvas;
}

void Penalty::setSender(const QString& ip, const QString& name) {
    _sender.ip = ip;
    _sender.name = name;
    rebuildSenderId();
}

QString Penalty::toString() const {
    return QString();
}

void Penalty::rebuildSenderId() {
    QStringList ip = _sender.ip.split('.');
    QString id = ip[0];
    if (ip.count()>3) id = ip[3];
    _senderId = QString("%1-%2").arg(_sender.name).arg(id);
}


void Penalty::setTimestamp(qint64 timestamp) {
    _timestamp = timestamp;
}

QJsonObject Penalty::jsonObject() const {

    QJsonObject obj;
    obj.insert("bib", QJsonArray() << _bib.first << _bib.second);
    obj.insert("gateId", _gateId);
    obj.insert("value", _value);
    obj.insert("timestamp", QString::number(_timestamp));
    QJsonObject sender;
    sender.insert("ip", _sender.ip);
    sender.insert("name", _sender.name);
    obj.insert("sender", sender);
    if (_canvas) {
        QJsonObject canvas;
        canvas.insert("teammate", _teammate);
        canvas.insert("spot1", QJsonArray() << _spot1.first << _spot1.second);
        canvas.insert("spot2", QJsonArray() << _spot2.first << _spot2.second);
        obj.insert("canvas", canvas);
    }

    return obj;
}



