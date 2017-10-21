#pragma once

#include "stdafx.h"

struct ProcConfig
{
	int ball_hue_lb;
	int ball_hue_ub;

	int car1_hue_lb;
	int car1_hue_ub;

	int car2_hue_lb;
	int car2_hue_ub;

	int car1_s_lb;
	int car2_s_lb;
	int ball_s_lb;
	int v_lb;
	int area_car_lb;
	int area_ball_lb;
};

class ImgProc
{
public:
	QVector<cv::Point2f> GetLocation();

	//���� opencv ���ɻ��������������ʱ�ı����
	void InitCv();

	//��֤�����޸� mat
	void Locate(cv::Mat& mat);

	// ��λ�������
	ProcConfig config;
private:
	std::vector<cv::Point2f> ball_centers, car1_centers, car2_centers;
	//src : ��ֵ��ͼ��
	static std::vector<cv::Point2f> GetCenter(cv::Mat src, const ProcConfig & cfg, int nType);
	enum Types { CARA = 1, CARB = 2,BALL = 3 };
	cv::Mat src, hsv;

	// 8UC1, hsv �� h ����
	cv::Mat hue;

	// ��ֵ��ͼ��s >= s_lb && h >= h_lb
	cv::Mat mask_ball,mask_car1,mask_car2;

	// ��ֵ��ͼ��
	cv::Mat car1, car2, ball;

	// ��ɫͼ�񣬽� car1 car2 ball ����һ�𣬷�����ԣ��ֱ�ΪBGRͨ����
	cv::Mat merged;

	cv::Mat dst;
};

class HighResCam : public QAbstractVideoSurface
{
	Q_OBJECT

private:
	ImgProc locateMachine;
	cv::Mat matPerspective;
	CoordinateConverter ccToLogic;
	void TransToLogic(QVector<cv::Point2f> &info);
public:
	HighResCam(QObject *parent = nullptr);
	~HighResCam();
	QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const override;
	bool present(const QVideoFrame &frame) override;
	void SetPerspective(const QVector<cv::Point2f> &pts);
signals:
	void ImageArrived(CameraInfo, QPixmap);
	void DebugPers(QString);
};
