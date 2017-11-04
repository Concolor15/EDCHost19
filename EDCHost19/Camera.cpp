#include "stdafx.h"
#include "Camera.h"

#include <QPixmap>

constexpr QSize sizeSet(1280, 720);

Camera *Camera::pInstance = nullptr;
using namespace cv;

void Camera::CameraProc(CameraInfo info, QPixmap pixShow)
{
	emit InfoReady(info,pixShow);
}

Camera::Camera()
{
	//Prepare a camera and a view
	auto theDevice = QCamera::availableDevices().at(QCamera::availableDevices().size() - 1);
	pCamWork = new QCamera(theDevice);
    //pCamWork->imageProcessing()->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceManual);
    //pCamWork->imageProcessing()->setManualWhiteBalance(4000);
	pHRView = new HighResCam;
	pCamWork->setViewfinder(pHRView);
	QCameraViewfinderSettings settings;
	settings.setResolution(sizeSet);
	pCamWork->setViewfinderSettings(settings);
	QObject::connect(pHRView, &HighResCam::ImageArrived, this, &Camera::CameraProc);
	QObject::connect(pHRView, &HighResCam::DebugPers, this, &Camera::DebugInfo);
}

Camera::~Camera()
{
	pCamWork->deleteLater();
	pHRView->deleteLater();
}

Camera * Camera::GetInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new Camera;
	}
	return pInstance;
}

void Camera::DestroyInstance()
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

void Camera::SetPerspecitve(const QVector<cv::Point2f>& pts)
{
	if (pts.size() != 4) return;
	pHRView->SetPerspective(pts);
}

void Camera::Begin()
{
	pCamWork->start();
}

void Camera::End()
{
	pCamWork->stop();
}

