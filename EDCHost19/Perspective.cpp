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
}

Perspective::~Perspective()
{
}

void Perspective::mousePressEvent(QMouseEvent * event)
{
	QPainter rndDrawer(this);
	auto pos = cv::Point(event->x(), event->y());
	auto centre = QPoint(pos.x, pos.y);
	if (pos.x >= 25 && 
		pos.x < 825 && 
		pos.y >= 175 && 
		pos.y < 775 &&
		event->button() == Qt::LeftButton)
	{
		rndDrawer.drawEllipse(centre, 3, 3);
		if (pos.y < 475)
		{
			if (pos.x < 425)
			{
				ptsSelected[0] = pos;
			}
			else
			{
				ptsSelected[1] = pos;
			}
		}
		else
		{
			if (pos.x < 425)
			{
				ptsSelected[2] = pos;
			}
			else
			{
				ptsSelected[3] = pos;
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
		if (pt == cv::Point(-1,-1))
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
		if (ptsSelected[i] != cv::Point(-1, -1))
		{
			lePositions[i]->setText(QString("(%1, %2)").arg(ptsSelected[i].x).arg(ptsSelected[i].y));
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
	QSize sizeOrigin = Camera::GetInstance()->GetCameraSize();
	float fXMax = static_cast<float>(sizeOrigin.width());
	float fYMax = static_cast<float>(sizeOrigin.height());
	for (auto &pt : ptsSelected)
	{
		pt.x = pt.x *  fXMax / 800;
		pt.y = pt.y *  fYMax / 600;
	}
	cv::Point2f ptSrc[4]
	{
		cv::Point2f{0, 0},
		cv::Point2f{ fXMax - 1, 0},
		cv::Point2f{0, fYMax - 1},
		cv::Point2f{ fXMax - 1, fYMax - 1}
	};
	cv::Point2f ptDst[4]{ ptsSelected[0],ptsSelected[1],ptsSelected[2],ptsSelected[3] };
	Camera::GetInstance()->SetPerspecitve(cv::getPerspectiveTransform(ptSrc, ptDst));
}

void Perspective::OnRevoke()
{
	ptsSelected = QVector<cv::Point>(4, cv::Point(-1, -1));
}
