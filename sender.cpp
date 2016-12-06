#include <QCoreApplication>
#include "pbuff.h"
#include "sender.h"


//Коммуникации между получателем и отправителем выведены на разные порты. Здесь отправитель сливает файл на порт "sendPort" и получает отчет о доставке через "checkPort".
Sender::Sender(int someCheckPort, int someSendPort, const QHostAddress& someGroupAddressTO, const QString& someTransportedFile, QObject *parent)
    : QUdpSocket(parent),checkPort(someCheckPort), sendPort(someSendPort),groupAddressTO(someGroupAddressTO), transportedFile(someTransportedFile)
{
    initSender();
}

void Sender::initSender()
{
    timer = new QTimer(this);

    this->bind(QHostAddress::AnyIPv4, checkPort, QUdpSocket::ShareAddress);
    this->joinMulticastGroup(groupAddressTO);


    QSettings settings(QString("%1\\%2").arg(QCoreApplication::applicationDirPath()).arg("UpdSenderSetting.ini"),QSettings::IniFormat);
    packetSize=settings.value("PacketSize","").toInt();
    if(packetSize==0)
    {
        settings.setValue("PacketSize",450);
        packetSize=450;
    }

    formQUeue(transportedFile);
    pendingPacket=dataToTransfer.takeFirst();
    sendDatagram(pendingPacket);
}

void Sender::sendDatagram(const UdpStream::UdpBytes& gPack)
{
    QByteArray bytesToSend=ProtoBytes<UdpStream::UdpBytes>::protoToByteArray(gPack);
    this->writeDatagram(bytesToSend.data(), bytesToSend.size(), groupAddressTO, sendPort);
}

void Sender::readIncome()
{
    QByteArray fileBytes;
    UdpStream::UdpBytes responceProto;
    while (this->hasPendingDatagrams())
    {
        fileBytes.resize(this->pendingDatagramSize());
        this->readDatagram(fileBytes.data(), fileBytes.size());
    }
    responceProto = ProtoBytes<UdpStream::UdpBytes>::protoFromByteArray(fileBytes);

    if(responceProto.packstatus()==UdpStream::UdpBytes::PackStatus::UdpBytes_PackStatus_LAST)
    {
        dataToTransfer.clear();
        timer->stop();
        emit finishedTransfer();
    }
    if(!dataToTransfer.isEmpty())
    {
        pendingPacket=dataToTransfer.takeFirst();
        resendTries=0;
        sending();
    }
}

void Sender::sending()
{
    if(resendTries<5)
    {
        sendDatagram(pendingPacket);
        qDebug()<<"dataToTransfer.count(): "<<dataToTransfer.count();
        timer->start(1000);
    }
    else
    {
        emit finishedTransfer();
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
        quint64 packNum=byteCount/packetSize;
        QString fileName=filePath.split("\\").takeLast();
        dataToTransfer<<newGPPacket(packetID,fileName.toLatin1(),UdpStream::UdpBytes::PackStatus::UdpBytes_PackStatus_FIRST);
        packetID++;
        for (int i =0;i<packNum;i++)
        {
            QByteArray tempBytes;
            for(int i1 =i*packetSize;i1<i*packetSize+packetSize;i1++)
            {
                tempBytes.append(rAll[i1]);
            }
            dataToTransfer<<newGPPacket(packetID,tempBytes,UdpStream::UdpBytes::PackStatus::UdpBytes_PackStatus_MIDDLE);
            packetID++;
        }
        int resid=byteCount%packetSize;
        QByteArray tempBytes="";
        if (resid>0)
        {
            int position=(packNum-resid+1);
            for(int i1 =0;i1<resid;i1++)
            {
                tempBytes.append(rAll[position+i1]);
            }
        }
        dataToTransfer<<newGPPacket(packetID,tempBytes,UdpStream::UdpBytes::PackStatus::UdpBytes_PackStatus_LAST);
    }
}


UdpStream::UdpBytes Sender::newGPPacket(int pID,const QByteArray& pBytes, UdpStream::UdpBytes::PackStatus pStatus)
{
    UdpStream::UdpBytes package;
    package.set_packid(pID);
    package.set_packcontent(pBytes.data(),pBytes.size());
    package.set_packstatus(pStatus);
    return package;
}
