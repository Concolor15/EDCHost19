#pragma once

#include <QDialog>
#include "ui_Perspective.h"

#include"stdafx.h"

class Perspective : public QDialog
{
	Q_OBJECT

public:
	Perspective(QPixmap pix, QWidget *parent = Q_NULLPTR);
	~Perspective();
protected:
	void mousePressEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
private:
	Ui::Perspective ui;
	QPixmap pixPerspect;
	QVector<cv::Point> ptsSelected;
	QVector<QLineEdit *> lePositions;
private slots:
	void OnConfirm();
	void OnRevoke();
};
