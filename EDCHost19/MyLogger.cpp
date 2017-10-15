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

void MyLogger::closeEvent(QCloseEvent * event)
{
	deleteLater();
	QDialog::closeEvent(event);
}

void MyLogger::OnCamDbg(QString qstrInfo)
{
	ui.teCamera->setText(qstrInfo);
}

void MyLogger::OnSerDbg(QString qstrInfo)
{
	ui.teSerial->setText(qstrInfo);
}
