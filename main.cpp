#include <QSettings>
#include <QTextStream>
#include <QDebug>
#include "sender.h"
#include <math.h>
#include <stdlib.h>
#include "pbuff.h"


//QUdpSocket socketSender;
//QUdpSocket socketReceiver;



//void readIncome()
//{
//    QByteArray fileBytes;
//    while (socketReceiver.hasPendingDatagrams())
//    {
//        QByteArray datagram;
//        datagram.resize(socketReceiver.pendingDatagramSize());
//        socketReceiver.readDatagram(datagram.data(), datagram.size());
//        fileBytes.append(datagram);
//    }
//    qDebug()<<fileBytes;
//}

//void readProtoIncome()
//{
//    QByteArray fileBytes;
//    QByteArray bytesFromProto;
//    while (socketReceiver.hasPendingDatagrams())
//    {
//        QByteArray datagram;
//        datagram.resize(socketReceiver.pendingDatagramSize());
//        socketReceiver.readDatagram(datagram.data(), datagram.size());
//        fileBytes.append(datagram);
//    }
//    UdpStream::UdpBytes gPackOut=ProtoBytes<UdpStream::UdpBytes>::protoFromByteArray(fileBytes);
//    bytesFromProto.append(gPackOut.packcontent().data());
//    qDebug()<<bytesFromProto;
//}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
//    QSettings settings(QString("%1\\%2").arg(QCoreApplication::applicationDirPath()).arg("UpdSenderSetting.ini"),QSettings::IniFormat);
//    settings.setValue("PacketSize",1000);
//    Sender sender(45455,45454,QHostAddress("239.255.43.21"),"C:\\Users\\Home\\Desktop\\Ted Irens - Sunday Breakfast.mp3");
    QFile myFile("C:\\Users\\Home\\Desktop\\Result\\Ted Irens - Sunday Breakfast.mp3");
    myFile.open(QIODevice::ReadWrite);




    UdpStream::UdpBytes gPackIn,gPackOut;
    QByteArray bytesBytes = myFile.readAll();

    QString someString;
    QTextStream opas(&someString);
    opas<<bytesBytes;

    gPackIn.set_packcontent(bytesBytes);
    gPackIn.set_packid(56);
    gPackIn.set_packstatus(UdpStream::UdpBytes::PackStatus::UdpBytes_PackStatus_MIDDLE);
    QByteArray tempBytes=ProtoBytes<UdpStream::UdpBytes>::protoToByteArray(gPackIn);

////    UdpStream::UdpBytes gPackOut;
////    QByteArray bytesFromProto;

//    QHostAddress groupAddress=QHostAddress("239.255.43.21");
//    socketReceiver.bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress);
//    socketReceiver.joinMulticastGroup(groupAddress);


////    //simpleStuff;
////    socketSender.writeDatagram(bytesBytes.data(), bytesBytes.size(), groupAddress, 45454);

//    //protoStuff;
//    socketSender.writeDatagram(tempBytes.data(), tempBytes.size(), groupAddress, 45454);

////    readIncome();
//    readProtoIncome();

//    for(int i=0; i<bytesBytes.count();i++)
//    {
//        opas<<bytesBytes[i];
//    }

    gPackOut=ProtoBytes<UdpStream::UdpBytes>::protoFromByteArray(tempBytes);
//    qDebug()<<"bytesBytes: "<<bytesBytes;
    QByteArray SometempBytes=gPackIn.packcontent().data();
    qDebug()<<"gPackOut.packcontent(): "<<bytesBytes;
//    myFile.write(myBytespROTO);
    myFile.close();
    return app.exec();

}
