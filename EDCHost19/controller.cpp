#include "stdafx.h"
#include "config.h"
#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent),
    timer(this),
    sp(this),
    data_buffer(32, 0),
    buffer_has_data(false)
{
    sp.setPortName(SERIAL_PORT_NAME);
    sp.setBaudRate(QSerialPort::Baud115200);
    sp.setDataBits(QSerialPort::Data8);
    sp.setParity(QSerialPort::NoParity);
    sp.setFlowControl(QSerialPort::NoFlowControl);
    sp.setStopBits(QSerialPort::OneStop);

    if (!sp.open(QIODevice::ReadWrite)) {
        qInfo() << sp.error();
    }

    QObject::connect(&timer, &QTimer::timeout, this, &Controller::serialport_timer_handle);
    timer.start(100);
}

void Controller::serialport_timer_handle()
{
    if (buffer_has_data)
    {
        buffer_has_data = false;

        sp.write(data_buffer);

        QString debugInfo = QTime::currentTime().toString() + "\n" + QString(data_buffer.toHex());
        emit SerialPortInfoUpdated(debugInfo);
    }
    else
    {
        emit SerialPortInfoUpdated("No Data Send");
    }
}

void Controller::sendLater(MatchInfo const& data)
{
    buffer_has_data = true;

    auto const& pos = data.posObjs;
    data_buffer[0] = 0xFC | (data.binShootout << 1) | (data.binSideShoot^1);
    data_buffer[1] = (data.quaGameStatus << 6) | (data.nTimeByRounds >> 8);
    data_buffer[2] = data.nTimeByRounds & 0xFF;

    data_buffer[3] = pos.posCar1.x();
    data_buffer[4] = pos.posCar1.y() >> 8;
    data_buffer[5] = pos.posCar1.y() & 0xFF;

    data_buffer[6] = pos.posCar2.x();
    data_buffer[7] = pos.posCar2.y() >> 8;
    data_buffer[8] = pos.posCar2.y() & 0xFF;

    data_buffer[9] = pos.posBall.x();
    data_buffer[10] = pos.posBall.y() >> 8;
    data_buffer[11] = pos.posBall.y() & 0xFF;

    data_buffer[12] = data.nHaltRound[0] >> 8;
    data_buffer[13] = data.nHaltRound[0] & 0xFF;
    data_buffer[14] = data.nHaltRound[1] >> 8;
    data_buffer[15] = data.nHaltRound[1] & 0xFF;
    data_buffer[16] = data.nEvil[0];
    data_buffer[17] = data.nEvil[1];
    data_buffer[18] = data.nScore[0];
    data_buffer[19] = data.nScore[1];
    data_buffer[30] = 0x0D;
    data_buffer[31] = 0x0A;
}
