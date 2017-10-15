#include "stdafx.h"
#include "Perspective.h"

Perspective::Perspective(QPixmap pix, QWidget *parent)
	: QDialog(parent),
	ptsSelected(4, cv::Point(-1, -1)),
	pixPerspect(pix)
{
	ui.setupUi(this);
	lePositions = { ui.lePosLU,ui.lePosRU,ui.lePosLD,ui.lePosRD };
	ui.lblPicture->setScaledContents(true);
	ui.lblPicture->setPixmap(pixPerspect);
}

Perspective::~Perspective()
{
}

void Perspective::mousePressEvent(QMouseEvent * event)
{
	QPainter rndDrawer(this);
	auto pos = cv::Point2f(event->x() - 25, event->y() - 175);
	auto centre = QPoint(pos.x, pos.y);
	if (pos.x >= 0 && 
		pos.x < 800 && 
		pos.y >= 0 && 
		pos.y < 600 &&
		event->button() == Qt::LeftButton)
	{
		rndDrawer.drawEllipse(centre, 3, 3);
		if (pos.y < 300)
		{
			if (pos.x < 400)
			{
				ptsSelected[0] = { 1280 * pos.x / 800,720 * pos.y / 600 };
			}
			else
			{
				ptsSelected[1] = { 1280 * pos.x / 800,720 * pos.y / 600 };
			}
		}
		else
		{
			if (pos.x < 400)
			{
				ptsSelected[2] = { 1280 * pos.x / 800,720 * pos.y / 600 };
			}
			else
			{
				ptsSelected[3] = { 1280 * pos.x / 800,720 * pos.y / 600 };
			}
		}
	}
	repaint();
	QDialog::mousePressEvent(event);
}

void Perspective::paintEvent(QPaintEvent * event)
{
	bool bReady = true, bEmpty = true;
	for (auto &pt : ptsSelected)
	{
		if (pt == cv::Point2f(-1,-1))
		{
			bReady = false;
		}
		else
		{
			bEmpty = false;
		}
	}
	ui.btnConfirm->setDisabled(!bReady);
	ui.btnRevoke->setDisabled(bEmpty);
	for (int i = 0; i < 4; ++i)
	{
		if (ptsSelected[i] != cv::Point2f(-1, -1))
		{
			lePositions[i]->setText(QString("(%1, %2)").arg(int(ptsSelected[i].x)).arg(int(ptsSelected[i].y)));
		}
		else
		{
			lePositions[i]->setText("");
		}
	}
	QDialog::paintEvent(event);
}

void Perspective::OnConfirm()
{
	Camera::GetInstance()->SetPerspecitve(ptsSelected);
}

void Perspective::OnRevoke()
{
	ptsSelected = QVector<cv::Point2f>(4, cv::Point2f(-1, -1));
}
