#include "stdafx.h"
#include "config.h"
#include "Perspective.h"
#include "Camera.h"

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
    int x = event->x() - 25;
    int y = event->y() - 175 - 90;
    if (x >= 0 && x < 1280 &&
        y >= 0 && y < 720 &&
		event->button() == Qt::LeftButton)
	{
		if (nSelected < ptsSelected.size())
		{
            ptsSelected[nSelected] = { x, y };
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
