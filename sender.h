#pragma once
#include <QtNetwork>
#include <QList>
#include "packageFormat.pb.h"

class Sender : public QUdpSocket
{
    Q_OBJECT
public:
    explicit Sender(int,int,const QHostAddress&,const QString&, QObject *parent = nullptr);
    void formQUeue(const QString&);
    void sendDatagram(const UdpStream::UdpBytes&);
    QTimer *timer= nullptr;
private slots:
    void readIncome();
    void sending();
    void initSender();
private:
    UdpStream::UdpBytes newGPPacket(int,const QByteArray&, UdpStream::UdpBytes::PackStatus);
    int checkPort, sendPort, packetID, resendTries=0, packetSize;
    QHostAddress groupAddressTO;
    QString transportedFile;
    QList<UdpStream::UdpBytes> dataToTransfer;
    QList<QByteArray> dataToTest;
    UdpStream::UdpBytes pendingPacket;
signals:
    void finishedTransfer();
};
