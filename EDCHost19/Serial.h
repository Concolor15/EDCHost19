#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>

#include "GlobalType.hpp"

class Serial final :public QObject
{
	Q_OBJECT
private:
	static Serial * pInstance;
	QSerialPort theSerial;
	Serial();
	~Serial();
public:
	static Serial * GetInstance();
	static void DestroyInstance();
	void Transmit(const SerialInfo &theData);
signals:
	void DebugInfo(QString);
};

