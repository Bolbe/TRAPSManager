#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include "View/dialogbox.h"
#include "View/filechooser.h"
#include "View/dialogboxqueue.h"

class ViewController : public QObject
{

    Q_OBJECT
    Q_PROPERTY(int fontSize READ fontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(QString folder READ folder CONSTANT)
    Q_PROPERTY(int appWindowWidth READ appWindowWidth CONSTANT)
    Q_PROPERTY(int appWindowHeight READ appWindowHeight CONSTANT)
    Q_PROPERTY(int bibCount READ bibCount NOTIFY bibCountChanged)


public:
    ViewController(DialogBoxQueue* dialogBoxQueue,
                   const QStringList& hostList,
                   int requestedTcpPort, int httpPort);

    int fontSize() const { return _fontSize; }
    QString statusText() const { return _statusText; }
    QString folder() const { return _folder; }
    int bibCount() const { return _bibCount; }

    int appWindowWidth() const { return _appWindowWidth; }
    int appWindowHeight() const { return _appWindowHeight; }

signals:

    void fontSizeChanged(int fontSize);
    void statusTextChanged(QString statusText);
    void quit();
    void popFileChooser(QString title, QStringList nameFilters);
    void requestPCE(QString& fileName, bool reset);
    void requestCSV(QString filename, bool reset);
    void requestPenaltyClear();
    void requestChronoClear();
    void toast(QString text, int delay); // delay in msec
    void requestTcpServer(QString host, int port);
    void selectedAddress(QString host);
    void bibCountChanged(int bibCount);
    void watchdog();
    void openSoftwareUpdate();
    void checknewVersion(bool force);
    void turnOnPleaseWait();
    void turnOffPleaseWait();


public slots:

    void incFontSize(int step);
    void setAppWindowWidth(int width);
    void setAppWindowHeight(int height);
    void setFolder(const QString &folder);
    void setStatusText(const QString& statusText);
    void openFileChooser(FileChooser* data);
    void selectedFilePath(QString filePath);
    void setBibCount(int bibCount);
    void about();
    void loadPCE();
    void loadCSV();
    void clearPenalties();
    void clearChronos();
    void printError(const QString& title, const QString& message);
    void broadcastError();
    void setTcpPort(int tcpPort);
    void viewReady(); // called when QML Component.onCompleted is called
    void tcpServerStartFailure();

private:

    int _fontSize;
    QString _statusText;
    QString _folder;
    QStringList _hostList;
    QString _selectedHost;
    int _runningTcpPort;
    int _requestedTcpPort;
    int _httpPort;
    int _bibCount;
    DialogBoxQueue* _dialogBoxQueue;
    FileChooser* _fileChooser;
    bool _fileChooserOpened;
    bool _showChrono;

    void refreshStatusText();

    int _appWindowWidth;
    int _appWindowHeight;
};

#endif // VIEWCONTROLLER_H
