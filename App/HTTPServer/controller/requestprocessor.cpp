#include "requestprocessor.h"

RequestProcessor::RequestProcessor() : stefanfrings::HttpRequestHandler() {


}

void RequestProcessor::service(stefanfrings::HttpRequest &request, stefanfrings::HttpResponse &response) {

    qDebug() << "Processing request " << request.getPath() << " from " << request.getPeerAddress().toString();
    QByteArray body = request.getBody();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Cannot parse json body :";
        qWarning() << body;
        qWarning() << parseError.errorString() << " at " << parseError.offset;
        jsonDoc = QJsonDocument();
    }
    if (jsonDoc.isNull() || jsonDoc.isEmpty()) {
        qWarning() << "No json body";
        jsonDoc = QJsonDocument();
    }

    QJsonDocument jsonResponse = process(jsonDoc);

    response.write(QJsonDocument(jsonResponse).toJson());

}
