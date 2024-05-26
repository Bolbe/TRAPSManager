#include "hellobroadcaster.h"
#include <QDebug>
#include <QUdpSocket>
#include <QDateTime>
#include <QSettings>
#include <QNetworkInterface>


#define BROADCAST_INTERVAL 3000

HelloBroadcaster::HelloBroadcaster() : QThread(),
    _tcpPort(0),
    _httpPort(0)
{

    moveToThread(this);
    _timer.moveToThread(this);
    _socket.moveToThread(this);
    connect(&_timer, SIGNAL(timeout()), SLOT(broadcast()));
    QObject::connect(this, &QThread::finished, &_timer, &QTimer::stop);
    QObject::connect(this, &QThread::finished, &_socket, &QUdpSocket::close);
    this->start();

}


void HelloBroadcaster::configure() {

    if (_address.isEmpty()) {
        qDebug() << "Interface address still empty. No broadcasting.";
        return;
    }
    if (_tcpPort < 1) {
        qDebug() << "Port for TCP server not set. No broadcasting";
        return;
    }
    if (_httpPort < 1) {
        qDebug() << "Port for HTTP server not set. No broadcasting";
        return;
    }

    qDebug() << "Starting HelloBroadcaster with UDP address: " << _address << " TCP port: " << _tcpPort << " HTTP port: "<< _httpPort;
    _timestamp = QDateTime::currentMSecsSinceEpoch() / 1000;
    _udpString = QString::number(_timestamp);
    _udpString.append(',');
    _udpString.append(_address);
    _udpString.append(',');
    _udpString.append(QString::number(_tcpPort));
    _udpString.append(',');
    _udpString.append(QString::number(_httpPort));
    _timer.start(BROADCAST_INTERVAL);

}

void HelloBroadcaster::broadcast() {

    QString strToBroadcast = QString("%0,%1").arg(_udpString).arg(QDateTime::currentMSecsSinceEpoch());
    //qDebug() << "Broadcasting: " << strToBroadcast;
    qint64 byteCount = _socket.writeDatagram(strToBroadcast.toLocal8Bit().constData(), QHostAddress::Broadcast, UDP_PORT);
    if (byteCount<0) emit broadcastError();
    emit sayHello();

}

QStringList HelloBroadcaster::localAddressList() {

    QStringList list;
    QList<QNetworkInterface> networkInterfaceList = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, networkInterfaceList) {

         // if interface is up and running and is not loopback
        if ((interface.flags() & (QNetworkInterface::IsRunning | QNetworkInterface::IsUp)) &&
            !(interface.flags() & QNetworkInterface::IsLoopBack)) {

            QList<QNetworkAddressEntry> addressList = interface.addressEntries();
            foreach (QNetworkAddressEntry addressEntry, addressList) {
                QHostAddress hostAddress = addressEntry.ip();
                if (hostAddress.toIPv4Address()) list.append(hostAddress.toString());
            }

        }
    }
    return list;

}

void HelloBroadcaster::setAddress(const QString &address) {
    _address = address;
    configure();
}

void HelloBroadcaster::setTcpPort(int tcpPort) {
    _tcpPort = tcpPort;
    configure();
}

void HelloBroadcaster::setHttpPort(int httpPort) {
    _httpPort = httpPort;
    configure();
}
