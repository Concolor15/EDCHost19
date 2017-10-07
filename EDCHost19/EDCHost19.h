#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EDCHost19.h"

#include "stdafx.h"
#include "Perspective.h"
#include "MatchMain.h"
#include "MyLogger.h"

class EDCHost19 : public QMainWindow
{
	Q_OBJECT

public:
	EDCHost19(QWidget *parent = Q_NULLPTR);
	~EDCHost19();
protected:
	void paintEvent(QPaintEvent * event) override;
	void closeEvent(QCloseEvent * event) override;
private:
	Ui::EDCHost19Class ui;
	QPixmap pixFromCamera;
private slots:
	void OnMatch();
	void OnPerspective();
	void OnDebug();
	void FetchImg(CameraInfo info, QPixmap pixGet);
signals:
	void PassInfo(CameraInfo, QPixmap);
};
