#include "penaltyrequestprocessor.h"
#include <QDebug>
#include <QByteArray>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>


QJsonDocument PenaltyRequestProcessor::process(const QJsonDocument &jsonDoc) {

    QJsonArray array = jsonDoc.array();
    QList<Penalty> penaltyList;
    foreach (QJsonValue json, array) {
        QJsonObject jsonPenalty = json.toObject();
        penaltyList.append(Penalty(jsonPenalty));
    }

    QJsonObject jsonResponse;

    if (penaltyList.count()>0) {
        emit incomingPenaltyList(penaltyList);
        jsonResponse.insert("response", 0);
    }
    else {
        jsonResponse.insert("response", 1);
    }

    return QJsonDocument(jsonResponse);

}
