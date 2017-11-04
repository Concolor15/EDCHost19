#pragma once

#include "config.h"
#include <QtCore/qpair.h>
#include <opencv2/core/types.hpp>

typedef unsigned short YType;
typedef unsigned char XType;
typedef QPair<XType, YType> Position;
typedef unsigned short RoundType;
typedef unsigned char ValType;



struct CameraInfo	//For Camera Only
{
	Position posBall, posCarA, posCarB;
    CameraInfo(const cv::Point2f &ptBall = { 0.,0. }, const cv::Point2f &ptCarA = { 0.,0. }, const cv::Point2f &ptCarB = { 0.,0. })
	{
		posBall.first = static_cast<XType>((int)ptBall.x);
		posBall.second = static_cast<YType>((int)ptBall.y);
		posCarA.first = static_cast<XType>((int)ptCarA.x);
		posCarA.second = static_cast<YType>((int)ptCarA.y);
		posCarB.first = static_cast<XType>((int)ptCarB.x);
		posCarB.second = static_cast<YType>((int)ptCarB.y);
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
	RoundType nAInBRounds;
	RoundType nBInARounds;
	RoundType nHaltA;
	RoundType nHaltB;
	constexpr YType GetLength() { return nLogicWidth; }
	constexpr XType GetWidth() { return nLogicHeight; }
	constexpr YType GetHalfLength() { return nLogicWidth / 2; }
	SerialInfo infoObjs;
};
