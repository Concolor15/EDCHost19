#pragma once

#include "config.h"
#include <QtCore/qpair.h>
#include <opencv2/core/types.hpp>

typedef unsigned short YType;
typedef unsigned char XType;
typedef QPair<XType, YType> Position;
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

//    CameraInfo(const cv::Point2f &ptBall = { 0.,0. }, const cv::Point2f &ptCarA = { 0.,0. }, const cv::Point2f &ptCarB = { 0.,0. })
//	{
//		posBall.first = static_cast<XType>((int)ptBall.x);
//		posBall.second = static_cast<YType>((int)ptBall.y);
//		posCarA.first = static_cast<XType>((int)ptCarA.x);
//		posCarA.second = static_cast<YType>((int)ptCarA.y);
//		posCarB.first = static_cast<XType>((int)ptCarB.x);
//		posCarB.second = static_cast<YType>((int)ptCarB.y);
//	}
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
	SIDE_A = 0b1,	//Left
	SIDE_B = 0b0	//Right
};

enum SHOOTOUT	//For binShootout
{
	YES = 0b1,
	NO = 0b0
};

struct SerialInfo	//For Serial Only
{
	RoundType nTimeByRounds;
	RoundType nHaltRoundA;
	RoundType nHaltRoundB;
	ValType nEvilA;
	ValType nEvilB;
	ValType nScoreA;
	ValType nScoreB;
	ValType quaGameStatus;
	ValType binShootout;
	ValType binSideShoot;
	CameraInfo posObjs;
};

struct MatchInfo	//For Logic
{
    RoundType nTimeByRounds;
    RoundType nHaltRoundA;
    RoundType nHaltRoundB;
    ValType nEvilA;
    ValType nEvilB;
    ValType nScoreA;
    ValType nScoreB;
    ValType quaGameStatus;
    ValType binShootout;
    ValType binSideShoot;
    CameraInfo posObjs;

    // above is needed to send to communication board through serialport

	RoundType nAInBRounds;
	RoundType nBInARounds;
	RoundType nHaltA;
	RoundType nHaltB;
	constexpr YType GetLength() { return nLogicWidth; }
	constexpr XType GetWidth() { return nLogicHeight; }
	constexpr YType GetHalfLength() { return nLogicWidth / 2; }
};
