#pragma once

#include <QtCore\qpair.h>
#include <opencv2\core\types.hpp>

typedef unsigned short YType;
typedef unsigned char XType;
typedef QPair<XType, YType> Position;
typedef unsigned short RoundType;
typedef unsigned char ValType;

typedef struct tagPos	//For Camera Only
{
	Position posBall, posCarA, posCarB;
	tagPos(const cv::Point &ptBall = { 0,0 }, const cv::Point &ptCarA = { 0,0 }, const cv::Point &ptCarB = { 0,0 })
	{
		posBall.first = static_cast<XType>(ptBall.x);
		posBall.second = static_cast<YType>(ptBall.y);
		posCarA.first = static_cast<XType>(ptCarA.x);
		posCarA.second = static_cast<YType>(ptCarA.y);
		posCarB.first = static_cast<XType>(ptCarB.x);
		posCarB.second = static_cast<YType>(ptCarB.y);
	}
}CameraInfo;

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

typedef struct tagSerial	//For Serial Only
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
}SerialInfo;

typedef struct tagMatch		//For Logic
{
	RoundType nAInBRounds;
	RoundType nBInARounds;
	RoundType nHaltA;
	RoundType nHaltB;
	constexpr YType GetLength() { return 297; }
	constexpr XType GetWidth() { return 210; }
	constexpr YType GetHalfLength() { return 297 / 2; }
	SerialInfo infoObjs;
}MatchInfo;
