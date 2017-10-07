#pragma once
#include "HighResCam.h"

#include "stdafx.h"

using std::vector;

class Camera final :public QObject
{
	Q_OBJECT
private:
	static Camera* pInstance;
	QCamera *pCamWork;
	HighResCam *pHRView;
	Camera();
	~Camera();
	cv::Mat matPerspective;
private slots:
	void CameraProc(CameraInfo info,QPixmap pixShow);
public:
	static Camera *GetInstance();
	static void DestroyInstance();
	void SetPerspecitve(const cv::Mat &persMat);
	cv::Mat GetPerspective();
	QSize GetCameraSize();
	void Begin();
	void End();
signals:
	void InfoReady(CameraInfo, QPixmap);
	void DebugInfo(QString);
};