#include "stdafx.h"
#include "Serial.h"
#include "config.h"

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

void Serial::Transmit(const SerialInfo & theData)
{
	byteToSend[0] = 0xFC | (theData.binShootout << 1);
	byteToSend[0] = byteToSend[0] | theData.binSideShoot;
	byteToSend[1] = (theData.quaGameStatus << 6) | (theData.nTimeByRounds >> 8);
	byteToSend[2] = theData.nTimeByRounds & 0xFF;
	byteToSend[3] = theData.posObjs.posCarA.first;
	byteToSend[4] = (theData.posObjs.posCarA.second >> 8);
	byteToSend[5] = (theData.posObjs.posCarA.second & 0xFF);
	byteToSend[6] = theData.posObjs.posCarB.first;
	byteToSend[7] = (theData.posObjs.posCarB.second >> 8);
	byteToSend[8] = (theData.posObjs.posCarB.second & 0xFF);
	byteToSend[9] = theData.posObjs.posBall.first;
	byteToSend[10] = (theData.posObjs.posBall.second >> 8);
	byteToSend[11] = (theData.posObjs.posBall.second & 0xFF);
	byteToSend[12] = (theData.nHaltRoundA >> 8);
	byteToSend[13] = (theData.nHaltRoundA & 0xFF);
	byteToSend[14] = (theData.nHaltRoundB >> 8);
	byteToSend[15] = (theData.nHaltRoundB & 0xFF);
	byteToSend[16] = theData.nEvilA;
	byteToSend[17] = theData.nEvilB;
	byteToSend[18] = theData.nScoreA;
	byteToSend[19] = theData.nScoreB;
	byteToSend[30] = 0x0D;
	byteToSend[31] = 0x0A;
}
