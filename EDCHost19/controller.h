#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtCore>
#include <QtSerialPort>
#include "GlobalType.h"
#include "MainLogic.h"

class Controller : public QObject
{
    Q_OBJECT
private:
    explicit Controller(QObject *parent = nullptr);
    static Controller* inst;

    MainLogic logic;

    QTimer timer;

    QSerialPort sp;
    QByteArray data_buffer;
    bool buffer_has_data;
    void serialport_timer_handle();

    void sendLater(MatchInfo const& info);
public:
    friend Controller* GetController()
    {
        return inst;
    }
    static void Init()
    {
        inst = new Controller();
    }
    static void Destroy()
    {
        delete inst;
    }

signals:
    void SerialPortInfoUpdated(QString info);
public slots:
};

#endif // CONTROLLER_H
