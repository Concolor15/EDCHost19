#pragma once

#include "config.h"
#include <QtCore>
#include <opencv2/core.hpp>

typedef unsigned short RoundType;
typedef unsigned char ValType;

inline QPointF point_cv2qt(cv::Point2f p)
{
    return {p.x, p.y};
}

struct CameraInfo	//For Camera Only
{
    //Position posBall, posCarA, posCarB;
    QPointF posBall;
    QPointF posCar[2];

    CameraInfo()
    {
        posBall = {0, 0};
        posCar[0] = {0, 0};
        posCar[1] = {0, 0};
    }

    CameraInfo(cv::Point2f ball, cv::Point2f car1, cv::Point2f car2)
    {
        posBall = point_cv2qt(ball);
        posCar[0] = point_cv2qt(car1);
        posCar[1] = point_cv2qt(car2);
    }
};


