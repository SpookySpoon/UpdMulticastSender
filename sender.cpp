#include <QtWidgets>
#include <QtNetwork>

#include "sender.h"

Sender::Sender(QObject *parent)
    : QObject(parent)
{
    groupAddressTO = QHostAddress("239.255.43.21");
//    groupAddressFROM = QHostAddress("239.255.43.21");
    timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);
//    udpSocket1 = new QUdpSocket(this);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(readIncome()));
    connect(this,SIGNAL(readyToSend()),this,SLOT(ultimateSend()));
    connect(timer,SIGNAL(timeout()),this,SLOT(ultimateSend()));
    udpSocket->bind(QHostAddress::AnyIPv4, 45455, QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(groupAddressTO);
    formQUeue();
//    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 2);
    readIncome();
}
void Sender::timerMark()
{
    qDebug()<<"Retried data sending...";

}

void Sender::startSending()
{
    timer->start(1000);
}

void Sender::sendDatagram(const QByteArray& opa)
{
//    qDebug()<<"Sending data";
    quint64 bS=0;
//    qDebug()<<"Bytes to send: "<<dataSent.count()<<" and size: "<<dataSent.size();
    bS=udpSocket->writeDatagram(opa.data(), opa.size(), groupAddressTO, 45454);
//    qDebug()<<"Bytes sent: "<<bS;
//    ++messageNo;
}

void Sender::readIncome()
{
    QByteArray fileBytes;
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        fileBytes.append(datagram);
    }
    qDebug()<<fileBytes;
    if(dataToTransfer.count()>0)
    {
        pendingBytesS=dataToTransfer.takeFirst();
        emit readyToSend();
    }
}
void Sender::ultimateSend()
{
    if(dataToTransfer.count()>0)
    {
        sendDatagram(pendingBytesS);
        qDebug()<<"dataToTransfer.count(): "<<dataToTransfer.count();
        timer->start(5000);
    }
    else
    {
        sendDatagram("{konetsN@hy%}");
        qDebug()<<"Sending final datagram";
    }
}

void Sender::formQUeue()
{
    QFile existingFile("C:/Users/Home/Desktop/Ted Irens - Sunday Breakfast.mp3");
    existingFile.open(QIODevice::ReadOnly);
    QByteArray rAll=existingFile.readAll();
    quint64 byteCount=rAll.count();
    quint64 packNum=byteCount/450;
    for (int i =0;i<packNum;i++)
    {
        QByteArray tempBytes;
        for(int i1 =i*450;i1<i*450+450;i1++)
        {
            tempBytes.append(rAll[i1]);
        }
        dataToTransfer<<tempBytes;
    }
    int resid=packNum%450;
    if (resid>0)
    {
        QByteArray lastArray;
        int position=(packNum-resid+1);

        for(int i1 =0;i1<resid;i1++)
        {
            lastArray.append(rAll[position+i1]);
        }
        dataToTransfer<<lastArray;
    }
    dataToTransfer<<"{konetsN@hy%}";
}
