#pragma once

#include <QDialog>
#include "ui_MatchMain.h"

class MatchMain : public QDialog
{
	Q_OBJECT

public:
	MatchMain(QWidget *parent = Q_NULLPTR);
	~MatchMain();
protected:
	void paintEvent(QPaintEvent * event) override;
	void closeEvent(QCloseEvent * event) override;
private:
	Ui::MatchMain ui;
private slots:
	void Running(CameraInfo infoReady, QPixmap pixShow);
	void OnPR();
	void OnBE();
	void OnSOA();
	void OnSOB();
	void OnPlusA();
	void OnPlusB();
	void OnPenaltyA();
	void OnPenaltyB();
};
