TARGET = TRAPSManager
TEMPLATE = app

QT += network qml quick sql concurrent
CONFIG += c++11

unix:!macx {
        QMAKE_LFLAGS += -no-pie
}

HEADERS += \
    Canoe/bibloader.h \
    Canoe/csvparser.h \
    Canoe/loadercsv.h \
    Canoe/loaderpce.h \
    Canoe/penaltyviewmodel.h \
    Canoe/trapsdb.h \
    HTTPServer/controller/biblistrequestprocessor.h \
    HTTPServer/controller/requestprocessor.h \
    View/simpleviewmodel.h \
    HTTPServer/requestmapper.h \
    HTTPServer/controller/dumpcontroller.h \
    HTTPServer/controller/formcontroller.h \
    HTTPServer/controller/fileuploadcontroller.h \
    View/dialogboxqueue.h \
    global.h \
    Network/hellobroadcaster.h \
    Canoe/bib.h \
    Canoe/event.h \
    database/database.h \
    Canoe/penalty.h \
    HTTPServer/controller/penaltyrequestprocessor.h \
    FFCanoe/ffcanoe.h \
    TCPServer/connectionhandler.h \
    TCPServer/connectionhandlerpool.h \
    TCPServer/tcpserver.h \
    View/viewcontroller.h \
    Network/hellobroadcaster.h \
    CompetFFCK/competffck.h \
    CompetFFCK/competffckconnector.h \
    FFCanoe/ffcanoeconnector.h \
    View/dialogbox.h \
    View/filechooser.h \
    Update/softwareupdate.h \
    Network/jsonnetworkmanager.h \
    Network/jsonnetworkreply.h \
    spreadsheet/spreadsheet.h \
    Canoe/biblistmodel.h \
    Canoe/penaltyset.h \
    Canoe/nickname.h


SOURCES += \
    Canoe/bibloader.cpp \
    Canoe/csvparser.cpp \
    Canoe/loadercsv.cpp \
    Canoe/loaderpce.cpp \
    Canoe/penaltyviewmodel.cpp \
    Canoe/trapsdb.cpp \
    HTTPServer/controller/biblistrequestprocessor.cpp \
    HTTPServer/controller/requestprocessor.cpp \
    View/simpleviewmodel.cpp \
    View/dialogboxqueue.cpp \
    main.cpp \
    HTTPServer/requestmapper.cpp \
    HTTPServer/controller/dumpcontroller.cpp \
    HTTPServer/controller/formcontroller.cpp \
    HTTPServer/controller/fileuploadcontroller.cpp \
    global.cpp \
    Network/hellobroadcaster.cpp \
    Canoe/bib.cpp \
    Canoe/event.cpp \
    database/database.cpp \
    Canoe/penalty.cpp \
    HTTPServer/controller/penaltyrequestprocessor.cpp \
    FFCanoe/ffcanoe.cpp \
    TCPServer/connectionhandler.cpp \
    TCPServer/connectionhandlerpool.cpp \
    TCPServer/tcpserver.cpp \
    View/viewcontroller.cpp \
    CompetFFCK/competffck.cpp \
    CompetFFCK/competffckconnector.cpp \
    FFCanoe/ffcanoeconnector.cpp \
    View/dialogbox.cpp \
    View/filechooser.cpp \
    Update/softwareupdate.cpp \
    Network/jsonnetworkreply.cpp \
    Network/jsonnetworkmanager.cpp \
    spreadsheet/spreadsheet.cpp \
    Canoe/biblistmodel.cpp \
    Canoe/penaltyset.cpp \
    Canoe/nickname.cpp



RESOURCES += \
    resources.qrc

RC_FILE = traps.rc
ICON = traps.icns

#---------------------------------------------------------------------------------------
# The following lines include the sources of the QtWebAppLib library
#---------------------------------------------------------------------------------------

include(QtWebApp/logging/logging.pri)
include(QtWebApp/httpserver/httpserver.pri)

DISTFILES +=

