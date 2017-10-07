#include "stdafx.h"
#include "EDCHost19.h"

EDCHost19::EDCHost19(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	MainLogic::GetInstance();
	Serial::GetInstance();
	Camera::GetInstance()->Begin();
	QObject::connect(Camera::GetInstance(), &Camera::InfoReady, this, &EDCHost19::FetchImg);
}

EDCHost19::~EDCHost19()
{
	Camera::GetInstance()->End();
	Camera::DestroyInstance();
	Serial::DestroyInstance();
	MainLogic::DestroyInstance();
}

void EDCHost19::paintEvent(QPaintEvent * event)
{
	QMainWindow::paintEvent(event);
}

void EDCHost19::closeEvent(QCloseEvent * event)
{
	QMainWindow::closeEvent(event);
}

void EDCHost19::OnMatch()
{
	MatchMain *pMatch = new MatchMain(this);
	pMatch->show();
	this->hide();
}

void EDCHost19::OnPerspective()
{
	Perspective persView(pixFromCamera, this);
	persView.exec();
}

void EDCHost19::OnDebug()
{
	MyLogger *pLog = new MyLogger(this);
	pLog->show();
}

void EDCHost19::FetchImg(CameraInfo info, QPixmap pixGet)
{
	pixFromCamera = pixGet.copy();
	emit PassInfo(info, pixFromCamera);
}

