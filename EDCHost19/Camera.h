#pragma once

#include <QPixmap>
#include "HighResCam.h"

using std::vector;

class Camera final :public QObject
{
	Q_OBJECT
private:
	static Camera* pInstance;

    HighResCam *pHRView;
	Camera();
	~Camera();
private slots:
	void CameraProc(CameraInfo info,QPixmap pixShow);
public:
    QCamera *pCamWork;

	static Camera *GetInstance();
	static void DestroyInstance();
	void SetPerspecitve(const QVector<cv::Point2f> &pts);
	void Begin();
	void End();
signals:
	void InfoReady(CameraInfo, QPixmap);
	void DebugInfo(QString);
};


