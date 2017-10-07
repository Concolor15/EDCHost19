#include "stdafx.h"
#include "Camera.h"

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
	pHRView = new HighResCam;
	pCamWork->setViewfinder(pHRView);
	QCameraViewfinderSettings settings;
	settings.setResolution(sizeSet);
	pCamWork->setViewfinderSettings(settings);
	QObject::connect(pHRView, &HighResCam::ImageArrived, this, &Camera::CameraProc);
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

void Camera::SetPerspecitve(const cv::Mat & persMat)
{
	matPerspective = persMat;
}

cv::Mat Camera::GetPerspective()
{
	return matPerspective;
}

QSize Camera::GetCameraSize()
{
	return pCamWork->viewfinderSettings().resolution();
}

void Camera::Begin()
{
	pCamWork->start();
}

void Camera::End()
{
	pCamWork->stop();
}

