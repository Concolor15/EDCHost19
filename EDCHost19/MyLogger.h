#pragma once

#include <QDialog>
#include "ui_MyLogger.h"

class MyLogger : public QDialog
{
	Q_OBJECT

public:
	MyLogger(QWidget *parent = Q_NULLPTR);
	~MyLogger();

private:
	Ui::MyLogger ui;
private slots:
	void OnCamDbg(QString qstrInfo);
	void OnSerDbg(QString qstrInfo);
};
