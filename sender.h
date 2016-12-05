#pragma once
#include <QObject>
#include <QList>
#include <QHostAddress>
#include "packageFormat.pb.h"

class QTimer;
class QUdpSocket;


class Sender : public QObject
{
    Q_OBJECT

public:
    explicit Sender(int,int,const QHostAddress&,const QString&, QObject *parent = nullptr);
    void formQUeue(const QString&);
    void sendDatagram(const udpStream::updBytes&);
private slots:
    void readIncome();
    void sending();
    void initSender();
private:
    int checkPort, sendPort, packetID, resendTries=0;
    QHostAddress groupAddressTO;
    QString transportedFile;

    udpStream::updBytes newGPPacket(int,const QByteArray&);
    QList<udpStream::updBytes> dataToTransfer;
    udpStream::updBytes dataSent, pendingPacket;
    QUdpSocket *udpSocket;
    QTimer *timer;
signals:
    void readyToSend();
};
