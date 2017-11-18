#pragma once

#include <QPixmap>
#include <QtMultimedia/QCamera>
#include <QAbstractVideoSurface>
#include <opencv2/core.hpp>
#include "GlobalType.h"
#include "CoordinateConverter.h"
#include "globalconfig.h"

struct LocateResult
{
    QPointF ball_center;
    QPointF cars_center[2];
    bool ball_succeeded;
    bool cars_succeeded;

    QPointF logic_ball_center;
    QPointF logic_cars_center[2];

    QTime timestamp;

    // below is for debug
};

class ImgProc
{
public:
    ImgProc() { }

	//利用 opencv 生成滑动条，方便调试时改变参数
	void InitCv();

	//保证不会修改 mat
	void Locate(cv::Mat& mat);

    //得到定位结果，指针由调用者负责释放
    LocateResult* GetResult();

	// 定位所需参数
    ProcConfig const* config;
    CoordinateConverter const* cvt;

//private:
	std::vector<cv::Point2f> ball_centers, car1_centers, car2_centers;
	//src : 二值化图像
	static std::vector<cv::Point2f> GetCenter(cv::Mat src, const ProcConfig & cfg, int nType);
	enum Types { CARA = 1, CARB = 2,BALL = 3 };
	cv::Mat src, hsv;

	// 8UC1, hsv 的 h 部分
	cv::Mat hue;

	// 单值化图像，s >= s_lb && h >= h_lb
	cv::Mat mask_ball,mask_car1,mask_car2;

	// 单值化图像
	cv::Mat car1, car2, ball;

	// 彩色图像，将 car1 car2 ball 合在一起，方便调试（分别为BGR通道）
	cv::Mat merged;

	cv::Mat dst;
};

