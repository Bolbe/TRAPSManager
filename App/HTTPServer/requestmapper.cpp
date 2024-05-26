#include <QCoreApplication>
#include "requestmapper.h"
#include "filelogger.h"
#include "controller/dumpcontroller.h"
#include "controller/formcontroller.h"
#include "controller/fileuploadcontroller.h"

RequestMapper::RequestMapper(stefanfrings::StaticFileController* staticFileController,
                             PenaltyRequestProcessor* penaltyProcessor,
                             BibListRequestProcessor* bibListProcessor)
    :stefanfrings::HttpRequestHandler(),
      _staticFileController(staticFileController),
      _penaltyProcessor(penaltyProcessor),
      _bibListProcessor(bibListProcessor)
{
    qDebug("RequestMapper: created");
}


RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}


void RequestMapper::service(stefanfrings::HttpRequest& request, stefanfrings::HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    if (path.startsWith("/penalty")) {
        _penaltyProcessor->service(request, response);
    }
    else if (path.startsWith("/biblist")) {
        _bibListProcessor->service(request, response);
    }
    else if (path.startsWith("/dump")) {
        DumpController().service(request, response);
    }

    else if (path.startsWith("/form")) {
        FormController().service(request, response);
    }

    else if (path.startsWith("/file")) {
        FileUploadController().service(request, response);
    }

    // All other paths are mapped to the static file controller.
    // In this case, a single instance is used for multiple requests.
    else {
        _staticFileController->service(request, response);
    }

    qDebug("RequestMapper: finished request");

}
