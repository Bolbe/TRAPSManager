#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QByteArray>
#include <QTime>
#include <QDir>
#include "staticfilecontroller.h"
#include "HTTPServer/requestmapper.h"
#include "global.h"
#include "Update/softwareupdate.h"
#include "Network/hellobroadcaster.h"
#include "Canoe/penalty.h"
#include <QQmlContext>
#include "FFCanoe/ffcanoe.h"
#include "HTTPServer/controller/penaltyrequestprocessor.h"
#include "QtWebApp/httpserver/httplistener.h"
#include "TCPServer/tcpserver.h"
#include "View/viewcontroller.h"
#include "CompetFFCK/competffck.h"
#include <QThread>
#include <QStandardPaths>
#include <Canoe/biblistmodel.h>
#include <QCommandLineParser>
#include "Canoe/penaltyviewmodel.h"
#include "Canoe/trapsdb.h"
#include "Canoe/loaderpce.h"


void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray stringDate = QTime::currentTime().toString("hh:mm:ss.zzz").toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stdout, "DEBUG|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stdout);
        break;
    case QtInfoMsg:
        fprintf(stdout, "INFO|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stdout);
        break;
    case QtWarningMsg:
        fprintf(stdout, "WARNING|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stdout);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "CRITICAL|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stderr);
        break;
    case QtFatalMsg:
        fprintf(stderr, "FATAL|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stderr);
        abort();
    }
    fflush(stderr);
}


int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("TRAPS");
    QGuiApplication::setOrganizationDomain("traps-ck.com");
    QGuiApplication::setApplicationName("TRAPSManager");

    qInfo() << "Platform name: " << QGuiApplication::platformName();

    qInstallMessageHandler(messageHandler);
    // the following data structures can be transported with signal / slot
    qRegisterMetaType< QList<Penalty> >("QList<Penalty>");
    qRegisterMetaType< QList<int> >("QList<int>");
    qRegisterMetaType< QHash<int,int> >("QHash<int,int>");
    qRegisterMetaType< QPair<int,int> >("QPair<int,int>");

    QCommandLineParser cmdLineParser;
    QCommandLineOption nobroadcastingOption("nobroadcasting");
    cmdLineParser.addOption(nobroadcastingOption);
    cmdLineParser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    cmdLineParser.process(app);

    Global::init();
    QSettings customSettings(QString("%0/traps.ini").arg(Global::appDataDir), QSettings::IniFormat);
    int tcpPort = customSettings.value("tcp_port", QVariant(0)).toInt();
    qInfo() << "Requested port: " << tcpPort;


    // Configure static file controller
    QSettings fileSettings(Global::httpConfigFile, QSettings::IniFormat, &app);
    fileSettings.beginGroup("docroot");
    stefanfrings::StaticFileController staticFileController(&fileSettings, &app);

    // Configure and start the Http listener
    QSettings httpSettings(Global::httpConfigFile, QSettings::IniFormat, &app);
    httpSettings.beginGroup("listener");
    PenaltyRequestProcessor penaltyRequestProcessor;
    BibListRequestProcessor bibListRequestProcessor;
    RequestMapper requestMapper(&staticFileController, &penaltyRequestProcessor, &bibListRequestProcessor);
    stefanfrings::HttpListener httpListener(&httpSettings, &requestMapper, &app);
    qInfo() << "HTTP server listening on port " << httpListener.serverPort();

    // HelloBroacaster
    HelloBroadcaster hello;
    hello.setHttpPort(httpListener.serverPort());

    FFCanoe ffcanoe;
    CompetFFCK competFFCK;

    TRAPSDB database(Global::appDataDir+"/traps.db");
    PenaltyViewModel penaltyViewModel;
    LoaderPCE loaderPCE(&database);
    BibListModel bibListModel(&database, &penaltyViewModel);
    bibListRequestProcessor.setBibListModel(&bibListModel);

    QObject::connect(&bibListModel, &BibListModel::penaltyUpdate, &ffcanoe, &FFCanoe::sendPenalty);
    QObject::connect(&bibListModel, &BibListModel::chronoReceived, &ffcanoe, &FFCanoe::sendTime);
    QObject::connect(&bibListModel, &BibListModel::penaltyUpdate, &competFFCK, &CompetFFCK::sendPenalty);
    QObject::connect(&bibListModel, &BibListModel::chronoReceived, &competFFCK, &CompetFFCK::sendTime);


    QStringList hostList = HelloBroadcaster::localAddressList();

    qInfo() << "Available ip addresses on this machine: " << hostList.join(',');

    QThread tcpServerThread;
    TCPServer tcpServer(&bibListModel);
    QObject::connect(&tcpServerThread, &QThread::finished, &tcpServer, &QTcpServer::close);
    tcpServer.moveToThread(&tcpServerThread);

    SoftwareUpdate softwareUpdate(QGuiApplication::platformName(), QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    DialogBoxQueue dialogBoxQueue;
    ViewController viewController(&dialogBoxQueue, hostList, tcpPort, httpListener.serverPort());

    // ViewController - softwareUpdater
    QObject::connect(&viewController, &ViewController::checknewVersion, &softwareUpdate, &SoftwareUpdate::checknewVersion);
    QObject::connect(&softwareUpdate, &SoftwareUpdate::triggerOpenSoftwareUpdate, &viewController, &ViewController::openSoftwareUpdate);
    QObject::connect(&softwareUpdate, &SoftwareUpdate::exitApp, &app, &QGuiApplication::quit);

    // ViewController -> Application
    QObject::connect(&viewController, &ViewController::quit, &app, &QGuiApplication::quit);


    // ViewController <-> LoaderPCE
    QObject::connect(&viewController, &ViewController::requestPCE, &loaderPCE, &LoaderPCE::processLoad);
    QObject::connect(&loaderPCE, &LoaderPCE::toast, &viewController, &ViewController::toast);
    QObject::connect(&loaderPCE, &LoaderPCE::error, &viewController, &ViewController::printError);

    // LoaderPCE -> BibList
    QObject::connect(&loaderPCE, &LoaderPCE::loaded, &bibListModel, &BibListModel::reload);
    QObject::connect(&loaderPCE, &LoaderPCE::loaded, &viewController, &ViewController::turnOffPleaseWait);

    // ViewController -> BibList
    QObject::connect(&viewController, &ViewController::requestCSV, &bibListModel, &BibListModel::loadCSV);
    QObject::connect(&viewController, &ViewController::requestPenaltyClear, &bibListModel, &BibListModel::clearPenalties);
    QObject::connect(&viewController, &ViewController::requestChronoClear, &bibListModel, &BibListModel::clearChronos);

    // ViewController -> TCPServer
    QObject::connect(&viewController, &ViewController::requestTcpServer, &tcpServer, &TCPServer::start);

    // TCPServer -> Viewcontroller
    QObject::connect(&tcpServer, &TCPServer::serverStarted, &viewController, &ViewController::setTcpPort);
    QObject::connect(&tcpServer, &TCPServer::serverStarted, &hello, &HelloBroadcaster::setTcpPort);
    QObject::connect(&tcpServer, &TCPServer::startFailure, &viewController, &ViewController::tcpServerStartFailure);

    // BibList -> ViewController
    QObject::connect(&bibListModel, &BibListModel::bibCountChanged, &viewController, &ViewController::setBibCount);
    QObject::connect(&bibListModel, &BibListModel::error, &viewController, &ViewController::printError);
    QObject::connect(&bibListModel, &BibListModel::toast, &viewController, &ViewController::toast);

    // PenaltyRequestProcessor -> BibList
    QObject::connect(&penaltyRequestProcessor, &PenaltyRequestProcessor::incomingPenaltyList,
                     &bibListModel, &BibListModel::processIncomingPenaltyList);

    // FFcanoe - ViewController
    QObject::connect(&ffcanoe, &FFCanoe::toast, &viewController, &ViewController::toast);
    QObject::connect(&ffcanoe, &FFCanoe::error, &viewController, &ViewController::printError);

    // CompetFFCK - ViewController
    QObject::connect(&competFFCK, &CompetFFCK::toast, &viewController, &ViewController::toast);
    QObject::connect(&competFFCK, &CompetFFCK::error, &viewController, &ViewController::printError);

    // Hello broadcaster <-> ViewController
    QObject::connect(&hello, &HelloBroadcaster::broadcastError, &viewController, &ViewController::broadcastError);
    QObject::connect(&hello, &HelloBroadcaster::sayHello, &viewController, &ViewController::watchdog);

    // Debug trick. Do not set address in some debug condition to avoid starting broadcasting.

    if (!cmdLineParser.isSet(nobroadcastingOption)) {
        QObject::connect(&viewController, &ViewController::selectedAddress, &hello, &HelloBroadcaster::setAddress);
    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("bibListModel", &bibListModel);
    engine.rootContext()->setContextProperty("ffcanoe", &ffcanoe);
    engine.rootContext()->setContextProperty("competFFCK", &competFFCK);
    engine.rootContext()->setContextProperty("softwareupdate", &softwareUpdate);
    engine.rootContext()->setContextProperty("penaltyViewModel", &penaltyViewModel);
    engine.rootContext()->setContextProperty("dialogBoxQueue", &dialogBoxQueue);
    engine.rootContext()->setContextProperty("viewcontroller", &viewController);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    tcpServerThread.start();
    viewController.setBibCount(bibListModel.bibCount());

    qDebug() << "Main thread is " << QThread::currentThread();

    app.exec();

    hello.exit();
    ffcanoe.exit();
    competFFCK.exit();
    tcpServerThread.exit();

    return 0;
}

