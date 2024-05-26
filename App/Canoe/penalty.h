#ifndef PENALTY_H
#define PENALTY_H

#include <QPair>
#include <QJsonObject>

#define PENALTY_0 0
#define PENALTY_2 2
#define PENALTY_50 50
#define PENALTY_NOTSET -1
#define PENALTY_CONFLICT 100

class Penalty {

public:

    struct Sender {
        QString ip;
        QString name;
    };

    Penalty();
    Penalty(QPair<int, int> bib, int gateId, int value);
    Penalty(const QJsonObject& json);
    Penalty(const Penalty& other);

    int value() const { return _value; }
    QString stringValue() const;

    bool canvas() const { return _canvas; }
    void setCanvas(bool canvas);

    QPair<double, double> spot1() const { return _spot1; }
    QPair<double, double> spot2() const { return _spot2; }
    void setSpot1(double x, double y);
    void setSpot2(double x, double y);

    bool teammate() const { return _teammate; }
    void setTeammate(bool teammate);

    qint64 timestamp() const { return _timestamp; }
    void setTimestamp(qint64 timestamp);

    int gateId() const { return _gateId; }

    QJsonObject jsonObject() const;

    QPair<int, int> bib() const { return _bib; }

    Sender sender() const { return _sender; }
    QString senderId() const { return _senderId; };
    void setSender(const QString& ip, const QString& name);

    QString toString() const;



private:

    QPair<int, int> _bib;
    Sender _sender;
    QString _senderId;
    int _gateId; // starts at 1
    int _value; // (-1, 0, 2, 50)
    QPair<double, double> _spot1;
    QPair<double, double> _spot2;

    bool _teammate;
    bool _canvas;
    qint64 _timestamp; // number of milliseconds since epoch

    void rebuildSenderId();

};

#endif // PENALTY_H
