#include "stdafx.h"
#include "config.h"

#include "Camera.h"
#include <QPixmap>

Camera *Camera::pInstance = nullptr;
using namespace cv;

void Camera::CameraProc(CameraInfo info, QPixmap pixShow)
{
	emit InfoReady(info,pixShow);
}

Camera::Camera()
{
	//Prepare a camera and a view
    auto devices = QCameraInfo::availableCameras();
    //pCamWork = new QCamera(devices[devices.size()-1]);
    //pCamWork->imageProcessing()->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceManual);
    //pCamWork->imageProcessing()->setManualWhiteBalance(4000);
	pHRView = new HighResCam;

    //QCameraViewfinderSettings settings;
    //settings.setResolution(CAMERA_SIZE);
    //pCamWork->setViewfinderSettings(settings);

    //pCamWork->setViewfinder(pHRView);

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
    assert(pts.size() == 4);
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

