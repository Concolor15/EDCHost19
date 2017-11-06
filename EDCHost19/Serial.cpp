#include "stdafx.h"
#include "config.h"
#include "Serial.h"

Serial * Serial::pInstance = nullptr;

Serial::Serial() : byteToSend(32, 0)
{
	//Serial Configurations
    theSerial.setPortName(SERIAL_PORT_NAME);
	theSerial.setBaudRate(QSerialPort::Baud115200);
	theSerial.setDataBits(QSerialPort::Data8);
	theSerial.setParity(QSerialPort::NoParity);
	theSerial.setFlowControl(QSerialPort::NoFlowControl);
	theSerial.setStopBits(QSerialPort::OneStop);

    if (!theSerial.open(QIODevice::ReadWrite)) {
        qInfo() << theSerial.error();
    }

	//Freq Control Configuration
	QObject::connect(&timerFreqCtrl, &QTimer::timeout, this, &Serial::Transmitter);
	timerFreqCtrl.start(100);
}

Serial::~Serial()
{
	timerFreqCtrl.stop();
	theSerial.close();
}

void Serial::Transmitter()
{
	theSerial.write(byteToSend);

    QString debugInfo = QTime::currentTime().toString() + QString(byteToSend.toHex());
    emit DebugInfo(debugInfo);
}

Serial * Serial::GetInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new Serial;
	}
	return pInstance;
}

void Serial::DestroyInstance()
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

void Serial::Transmit(const MatchInfo & data)
{
    auto const& pos = data.posObjs;
    byteToSend[0] = 0xFC | (data.binShootout << 1);
    byteToSend[0] = byteToSend[0] | (data.binSideShoot^1);
    byteToSend[1] = (data.quaGameStatus << 6) | (data.nTimeByRounds >> 8);
    byteToSend[2] = data.nTimeByRounds & 0xFF;

    byteToSend[3] = pos.posCar1.x();
    byteToSend[4] = pos.posCar1.y() >> 8;
    byteToSend[5] = pos.posCar1.y() & 0xFF;

    byteToSend[6] = pos.posCar2.x();
    byteToSend[7] = pos.posCar2.y() >> 8;
    byteToSend[8] = pos.posCar2.y() & 0xFF;

    byteToSend[9] = pos.posBall.x();
    byteToSend[10] = pos.posBall.y() >> 8;
    byteToSend[11] = pos.posBall.y() & 0xFF;

    byteToSend[12] = data.nHaltRound[0] >> 8;
    byteToSend[13] = data.nHaltRound[0] & 0xFF;
    byteToSend[14] = data.nHaltRound[1] >> 8;
    byteToSend[15] = data.nHaltRound[1] & 0xFF;
    byteToSend[16] = data.nEvil[0];
    byteToSend[17] = data.nEvil[1];
    byteToSend[18] = data.nScore[0];
    byteToSend[19] = data.nScore[1];
	byteToSend[30] = 0x0D;
	byteToSend[31] = 0x0A;
}
