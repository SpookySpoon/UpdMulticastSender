#pragma once
#include <QDialog>
#include <QHostAddress>

class QDialogButtonBox;
class QLabel;
class QPushButton;
class QTimer;
class QUdpSocket;
class QSpinBox;

class Sender : public QDialog
{
    Q_OBJECT

public:
    Sender(QWidget *parent = 0);


private slots:
    void ttlChanged(int newTtl);
    void startSending();
    void sendDatagram();

private:
    QLabel *statusLabel;
    QLabel *ttlLabel;
    QSpinBox *ttlSpinBox;
    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QUdpSocket *udpSocket,*udpSocket1,*udpSocket2;
    QTimer *timer;
    QHostAddress groupAddress;
    int messageNo;
};
