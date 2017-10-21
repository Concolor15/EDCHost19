#include "stdafx.h"
#include "Perspective.h"

Perspective::Perspective(QPixmap pix, QWidget *parent)
	: QDialog(parent),
	ptsSelected(4, cv::Point(-1, -1)),
	pixPerspect(pix)
{
	ui.setupUi(this);
	lePositions = { ui.lePosLU,ui.lePosRU,ui.lePosLD,ui.lePosRD };
	ui.lblPicture->setPixmap(pixPerspect);
	nSelected = 0;
}

Perspective::~Perspective()
{
}

void Perspective::mousePressEvent(QMouseEvent * event)
{
	auto pos = cv::Point2f(event->x() - 25, event->y() - 175);
	auto centre = QPoint(pos.x, pos.y);
	if (pos.x >= 0 &&
		pos.x < 1280 &&
		pos.y >= 0 &&
		pos.y < 720 &&
		event->button() == Qt::LeftButton)
	{
		if (nSelected < ptsSelected.size())
		{
			ptsSelected[nSelected] = { pos.x,pos.y };
			++nSelected;
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
	nSelected = 0;
}
