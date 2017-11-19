#pragma once

#include <QPixmap>
#include <QtMultimedia/QCamera>
#include <QAbstractVideoSurface>
#include <opencv2/core.hpp>
#include "type.h"
#include "CoordinateConverter.h"
#include "globalconfig.h"



class ImgProc
{
public:
    ImgProc() { }

    bool debugEnabled = false;

	//保证不会修改 mat
	void Locate(cv::Mat& mat);

    //得到定位结果，指针由调用者负责释放
    LocateResult* GetResult();

	// 定位所需参数
    ProcConfig const* config;
    CoordinateConverter const* cvt;

//private:
	std::vector<cv::Point2f> ball_centers, car1_centers, car2_centers;
    //v2 : 二值化图像
    std::vector<cv::Point2f> GetCenter(cv::Mat v2, int nType);
	enum Types { CARA = 1, CARB = 2,BALL = 3 };

	// 单值化图像
	cv::Mat car1, car2, ball;

	// 彩色图像，将 car1 car2 ball 合在一起，方便调试（分别为BGR通道）
	cv::Mat merged;

	cv::Mat dst;
};

