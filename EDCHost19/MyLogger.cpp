#include "stdafx.h"
#include "MyLogger.h"

MyLogger::MyLogger(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	QObject::connect(Camera::GetInstance(), &Camera::DebugInfo, this, &MyLogger::OnCamDbg);
	QObject::connect(Serial::GetInstance(), &Serial::DebugInfo, this, &MyLogger::OnSerDbg);
}

MyLogger::~MyLogger()
{
}

void MyLogger::OnCamDbg(QString qstrInfo)
{
	ui.teCamera->append(qstrInfo);
}

void MyLogger::OnSerDbg(QString qstrInfo)
{
	ui.teSerial->append(qstrInfo);
}
