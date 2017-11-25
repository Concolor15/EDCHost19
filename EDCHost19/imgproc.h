#pragma once

#include <QPixmap>
#include <QtMultimedia/QCamera>
#include <QAbstractVideoSurface>
#include <opencv2/core.hpp>
#include "locate.h"
#include "CoordinateConverter.h"
#include "globalconfig.h"



class ImgProc
{
public:
    ImgProc() { }

    bool debugEnabled = false;

	//保证不会修改 mat
    //返回值由调用者负责释放
    LocateResult* Locate(cv::Mat const& mat, int64_t timestamp_ms);

	// 定位所需参数
    ProcConfig const* config;
    CoordinateConverter const* cvt;

//private:

    //二值化
    void binarize(cv::Mat const& src);

    //获取结果
    void genResult(LocateResult* r, int64_t timestamp_ms);

    std::vector<cv::Point2f> ball_centers, cars_centers[2];
    //v2 : 二值化图像
    std::vector<cv::Point2f> GetCenter(cv::Mat v2, int nType);
	enum Types { CARA = 1, CARB = 2,BALL = 3 };

    ObjectTracker ball_tracker;
    ObjectTracker car_tracker[2];

	// 单值化图像
	cv::Mat car1, car2, ball;

	// 彩色图像，将 car1 car2 ball 合在一起，方便调试（分别为BGR通道）
	cv::Mat merged;

	cv::Mat dst;
};

