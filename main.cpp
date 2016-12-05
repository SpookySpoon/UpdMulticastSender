#include <QApplication>

#include "sender.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Sender sender(45455,45454,QHostAddress("239.255.43.21"),"C:\\Users\\Home\\Desktop\\Ted Irens - Sunday Breakfast.mp3");
    return app.exec();

}
