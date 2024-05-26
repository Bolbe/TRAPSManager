#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H

#include <QObject>
#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
#include <QJsonDocument>

class RequestProcessor : public stefanfrings::HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(RequestProcessor)

public:
    explicit RequestProcessor();

    // HttpRequestHandler interface
public:
    void service(stefanfrings::HttpRequest &request, stefanfrings::HttpResponse &response);


protected:

    virtual QJsonDocument process(const QJsonDocument& jsonDoc) = 0;

};

#endif // REQUESTPROCESSOR_H
