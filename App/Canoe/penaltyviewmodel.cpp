#include "penaltyviewmodel.h"

const QString PenaltyViewModel::title = "title";
const QString PenaltyViewModel::sender = "sender";
const QString PenaltyViewModel::timestamp = "timestamp";
const QString PenaltyViewModel::teammate = "teammate";
const QString PenaltyViewModel::xspot1 = "xspot1";
const QString PenaltyViewModel::yspot1 = "yspot1";
const QString PenaltyViewModel::xspot2 = "xspot2";
const QString PenaltyViewModel::yspot2 = "yspot2";

PenaltyViewModel::PenaltyViewModel()
{
    QVariantMap map;
    map.insert(title, "Title");
    map.insert(sender, "Sender");
    map.insert(timestamp, "Timestamp");
    map.insert(xspot1, 0.3);
    map.insert(yspot1, 0.3);
    map.insert(xspot2, 0.7);
    map.insert(yspot2, 0.7);
    map.insert(teammate, false);
    set(map);
}
