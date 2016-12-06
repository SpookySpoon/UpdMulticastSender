#include <QThread>
#include <QSettings>
#include "sender.h"
#include "deliveryhandler.h"


//Решил поиграться с потоками, чтобы если придется делать сервер, то он мог бы для каждой отправки отдельный поток создавать.
DeliveryHandler::DeliveryHandler()
{
    QSettings settings(QString("%1\\%2").arg(QCoreApplication::applicationDirPath()).arg("UpdSenderSetting.ini"),QSettings::IniFormat);
//    settings.setValue("PacketSize",1000);


    QThread* newThread=new QThread;
    Sender* sender=new Sender(45455,45454,QHostAddress("239.255.43.21"),"C:\\Users\\Home\\Desktop\\Ted Irens - Sunday Breakfast.mp3");

    QObject::connect(sender,SIGNAL(readyRead()),sender,SLOT(readIncome()));
    QObject::connect(sender->timer,SIGNAL(timeout()),sender,SLOT(sending()));
    QObject::connect(sender,SIGNAL(finishedTransfer()),sender,SLOT(deleteLater()));
    QObject::connect(sender,SIGNAL(destroyed(QObject*)),newThread,SLOT(quit()));
    QObject::connect(newThread,SIGNAL(finished()),newThread,SLOT(deleteLater()));
    QObject::connect(newThread,SIGNAL(started()),sender,SLOT(initSender()));

    sender->moveToThread(newThread);
    newThread->start();
}
