#pragma once

#include <QDialog>
#include "ui_Perspective.h"

#include <opencv2/core.hpp>

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
	QVector<cv::Point2f> ptsSelected;
	QVector<QLineEdit *> lePositions;
	int nSelected;
private slots:
	void OnConfirm();
	void OnRevoke();
};
