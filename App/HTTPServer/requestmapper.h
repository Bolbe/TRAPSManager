/**
  @file
  @author Stefan Frings
*/

#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "httprequesthandler.h"
#include "staticfilecontroller.h"
#include "controller/penaltyrequestprocessor.h"
#include "controller/biblistrequestprocessor.h"

/**
  The request mapper dispatches incoming HTTP requests to controller classes
  depending on the requested path.
*/

class RequestMapper : public stefanfrings::HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(RequestMapper)
public:

    /**
      Constructor.
      @param parent Parent object
    */
    RequestMapper(stefanfrings::StaticFileController* staticFileController,
                  PenaltyRequestProcessor* penaltyProcessor,
                  BibListRequestProcessor* bibListProcessor);

    /**
      Destructor.
    */
    ~RequestMapper();

    /**
      Dispatch incoming HTTP requests to different controllers depending on the URL.
      @param request The received HTTP request
      @param response Must be used to return the response
    */
    void service(stefanfrings::HttpRequest& request, stefanfrings::HttpResponse& response);

private:

    stefanfrings::StaticFileController* _staticFileController;
    PenaltyRequestProcessor* _penaltyProcessor;
    BibListRequestProcessor* _bibListProcessor;

};

#endif // REQUESTMAPPER_H
