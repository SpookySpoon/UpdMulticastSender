#include <QCoreApplication>

#include "deliveryhandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    DeliveryHandler opaHandler;

    return app.exec();

}
