#include <QtWidgets>
#include <QtNetwork>

#include "pbuff.h"
#include "sender.h"

Sender::Sender(int someCheckPort, int someSendPort, const QHostAddress& someGroupAddressTO, const QString& someTransportedFile, QObject *parent)
    : QObject(parent),checkPort(someCheckPort), sendPort(someSendPort),groupAddressTO(someGroupAddressTO), transportedFile(someTransportedFile)
{
    initSender();
}
void Sender::initSender()
{
    timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(readIncome()));
    connect(this,SIGNAL(readyToSend()),this,SLOT(sending()));
    connect(timer,SIGNAL(timeout()),this,SLOT(sending()));
    udpSocket->bind(QHostAddress::AnyIPv4, checkPort, QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(groupAddressTO);
    formQUeue(transportedFile);
    pendingPacket=dataToTransfer.takeFirst();
    sendDatagram(pendingPacket);
}

void Sender::sendDatagram(const udpStream::updBytes& gPack)
{
    QByteArray bytesToSend=ProtoBytes<udpStream::updBytes>::protoToByteArray(gPack);
    udpSocket->writeDatagram(bytesToSend.data(), bytesToSend.size(), groupAddressTO, sendPort);
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
    if(fileBytes=="stop")
    {
        dataToTransfer.clear();
        timer->stop();
    }
    if(dataToTransfer.count()>0)
    {
        pendingPacket=dataToTransfer.takeFirst();
        emit readyToSend();
        resendTries=0;
    }
}
void Sender::sending()
{
    if(resendTries<5)
    {
        if(dataToTransfer.count()>0)
        {
            sendDatagram(pendingPacket);
            qDebug()<<"dataToTransfer.count(): "<<dataToTransfer.count();
            timer->start(1000);
        }
        else
        {
            sendDatagram(newGPPacket(packetID-1,QString("{konetsN@hy%}").toLatin1()));
            qDebug()<<"Sending final datagram";
        }
    }
    resendTries++;
}


void Sender::formQUeue(const QString& filePath)
{
    packetID=0;
    QFile existingFile(filePath);
    if (existingFile.open(QIODevice::ReadOnly))
    {
        QByteArray rAll=existingFile.readAll();
        quint64 byteCount=rAll.count();
        quint64 packNum=byteCount/450;
        QString fileName=filePath.split("\\").takeLast();
        dataToTransfer<<newGPPacket(packetID,fileName.toLatin1());
        packetID++;
        for (int i =0;i<packNum;i++)
        {
            QByteArray tempBytes;
            for(int i1 =i*450;i1<i*450+450;i1++)
            {
                tempBytes.append(rAll[i1]);
            }
            dataToTransfer<<newGPPacket(packetID,tempBytes);
            packetID++;
        }
        int resid=byteCount%450;
        if (resid>0)
        {
            QByteArray tempBytes;
            int position=(packNum-resid+1);

            for(int i1 =0;i1<resid;i1++)
            {
                tempBytes.append(rAll[position+i1]);
            }
            dataToTransfer<<newGPPacket(packetID,tempBytes);
            packetID++;
        }
        dataToTransfer<<newGPPacket(packetID-1,"{konetsN@hy%}");
    }
}


udpStream::updBytes Sender::newGPPacket(int pID,const QByteArray& pBytes)
{
    udpStream::updBytes package;
    package.set_packid(pID);
    package.set_pack(pBytes);
    return package;
}
