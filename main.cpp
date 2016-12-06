#include <QSettings>
#include "sender.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QSettings settings(QString("%1\\%2").arg(QCoreApplication::applicationDirPath()).arg("UpdSenderSetting.ini"),QSettings::IniFormat);
    settings.setValue("PacketSize",1000);
    Sender sender(45455,45454,QHostAddress("239.255.43.21"),"C:\\Users\\Home\\Desktop\\Ted Irens - Sunday Breakfast.mp3");

    return app.exec();

}
