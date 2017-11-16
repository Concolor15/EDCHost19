#pragma once

#include "config.h"
#include <QtCore/qpair.h>
#include <opencv2/core/types.hpp>

typedef unsigned short RoundType;
typedef unsigned char ValType;

inline QPoint point_cv2qt(cv::Point2f p)
{
    return {(int)p.x, (int)p.y};
}

struct CameraInfo	//For Camera Only
{
    //Position posBall, posCarA, posCarB;
    QPoint posBall;
    QPoint posCar1;
    QPoint posCar2;
    CameraInfo()
    {
        posBall = {0, 0};
        posCar1 = {0, 0};
        posCar2 = {0, 0};
    }

    CameraInfo(cv::Point2f ball, cv::Point2f car1, cv::Point2f car2)
    {
        posBall = point_cv2qt(ball);
        posCar1 = point_cv2qt(car1);
        posCar2 = point_cv2qt(car2);
    }
};

enum PHASE		//For quaGameStatus
{
	NOTBEGIN = 0b00,
	RUNNING = 0b01,
	PAUSE = 0b10,
	OVER = 0b11
};

enum SIDE	//For binSideShoot
{
    SIDE_A = 0b0,   //Left
    SIDE_B = 0b1,   //Right
};

enum SHOOTOUT	//For binShootout
{
	YES = 0b1,
	NO = 0b0
};

struct MatchInfo	//For Logic
{
    int nTimeByRounds;
    int nHaltRound[2];
    int nEvil[2];
    int nScore[2];
    PHASE quaGameStatus;
    bool binShootout;
    int shootSide;
    CameraInfo posObjs;

    // below should NOT be send to communication board through serialport

    int nInOpRound[2];
    int nHalt[2];

    static constexpr int LENGTH = nLogicWidth;
    static constexpr int WIDTH = nLogicHeight;
    static constexpr int HALF_LENGTH = nLogicWidth / 2;
};
