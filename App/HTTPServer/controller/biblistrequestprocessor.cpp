#include "biblistrequestprocessor.h"
#include <QDebug>
#include <QJsonArray>
#include "Canoe/biblistmodel.h"
#include <QDateTime>


BibListRequestProcessor::BibListRequestProcessor() : RequestProcessor(),
    _bibList(nullptr)
{

}

QJsonDocument BibListRequestProcessor::process(const QJsonDocument &jsonDoc) {

    QJsonObject response;
    QJsonArray jsonArray;

    if (_bibList) {

        QJsonArray array;
        // reading Biblist model. Blocks until call is done. Use invokeMethod to be thread safe.
        qDebug() << "Invoking jsonArray on bib list model";
        QMetaObject::invokeMethod(_bibList, "jsonArray", Qt::BlockingQueuedConnection, Q_RETURN_ARG(QJsonArray, array));

        jsonArray = array;

    }

    response.insert("bibList", jsonArray);
    response.insert("timestamp", QDateTime::currentMSecsSinceEpoch());

    return QJsonDocument(response);

}
