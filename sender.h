#pragma once
#include <QObject>
#include <QList>
#include <QHostAddress>


class QTimer;
class QUdpSocket;


class Sender : public QObject
{
    Q_OBJECT

public:
    Sender(QObject *parent = 0);

    void formQUeue();
private slots:
    void startSending();
    void sendDatagram(const QByteArray&);
    void readIncome();
    void ultimateSend();
    void timerMark();
private:
    QList<QByteArray> dataToTransfer;
    QByteArray dataSent, pendingBytesS;
    QUdpSocket *udpSocket,*udpSocket1,*udpSocket2;
    QTimer *timer;
    QHostAddress groupAddressTO;
    int messageNo;
signals:
    void readyToSend();
};
