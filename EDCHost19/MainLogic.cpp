#include "stdafx.h"
#include "config.h"
#include "GlobalType.h"
#include "MainLogic.h"

constexpr int thePenalty = 50;

MainLogic::MainLogic()
{
	ResetInfo();
}

MainLogic::~MainLogic()
{
	
} 

void MainLogic::ResetInfo()
{
	//Set all data to init
    info.nAInBRounds = 0;
    info.nBInARounds = 0;
    info.binShootout = SHOOTOUT::NO;
    info.binSideShoot = SIDE::SIDE_B;
    info.nEvilA = 0;
    info.nEvilB = 0;
    info.nHaltRoundA = 0;
    info.nHaltRoundB = 0;
    info.nScoreA = 0;
    info.nScoreB = 0;
    info.nTimeByRounds = 0;
    info.posObjs = CameraInfo();
    info.quaGameStatus = PHASE::NOTBEGIN;
    info.nHaltA = 0;
    info.nHaltB = 0;
}

void MainLogic::EvaluateEvil()
{
	//Running to calculate
    if (info.quaGameStatus != PHASE::RUNNING) return;
    if (info.binShootout == SHOOTOUT::YES) return;
    const auto &posA = info.posObjs.posCar1;
    const auto &posB = info.posObjs.posCar2;
    if (posA.y() > info.GetHalfLength() && info.nHaltRoundA != 0)
	{
        ++info.nAInBRounds;
        if (info.nAInBRounds % nFPS == 0 && info.nHaltRoundA != 0)
		{
            auto d = info.GetLength() - posA.y();
            info.nEvilA += 5 - d / 30;
		}
	}
    if (posB.y() <= info.GetHalfLength() && info.nHaltRoundB != 0)
	{
        ++info.nBInARounds;
        if (info.nBInARounds % nFPS == 0)
		{
            auto d = posB.y() - 0;
            info.nEvilB += 5 - d / 30;
		}
	}
}

void MainLogic::ResetEvil(int nSide)
{
	if (nSide == SIDE::SIDE_A)
	{
        info.nAInBRounds = 0;
        info.nEvilA = 0;
	}
	if (nSide == SIDE::SIDE_B)
	{
        info.nBInARounds = 0;
        info.nEvilB = 0;
	}
}

void MainLogic::MatchBegin()
{
	ResetInfo();
    info.quaGameStatus = PHASE::RUNNING;
}

void MainLogic::MatchEnd()
{
    info.quaGameStatus = PHASE::OVER;
	ResetInfo();
}

void MainLogic::MatchPause()
{
    info.quaGameStatus = PHASE::PAUSE;
}

void MainLogic::MatchResume()
{
    info.quaGameStatus = PHASE::RUNNING;
}

void MainLogic::Run(const CameraInfo & pts)
{
	//Update position info
    info.posObjs = pts;
	//Update Evil
	EvaluateEvil();
	//Deal with penalty of halting
    if (info.binShootout == SHOOTOUT::NO && info.quaGameStatus == PHASE::RUNNING)
	{
        ++info.nTimeByRounds;
        if (info.nHaltRoundA == 0)
		{
            if (info.nEvilA >= 100)
			{
                auto nStopSecA = MAX((5 + 2 * info.nHaltA), 15);
                info.nHaltRoundA = nStopSecA * nFPS;
                ++info.nHaltA;
			}
		}
		else
		{
            if (info.nHaltRoundA == 1)
			{
				ResetEvil(SIDE_A);
			}
            --info.nHaltRoundA;
		}
        if (info.nHaltRoundB == 0)
		{
            if (info.nEvilB >= 100)
			{
                auto nStopSecB = MAX((5 + 2 * info.nHaltB), 15);
                info.nHaltRoundB = nStopSecB * nFPS;
                ++info.nHaltB;
			}
		}
		else
		{
            if (info.nHaltRoundB == 1)
			{
				ResetEvil(SIDE_B);
			}
            --info.nHaltRoundB;
		}
	}
	//Time up
    if (info.nTimeByRounds / nFPS == GAME_TIME &&
        info.binShootout == SHOOTOUT::NO &&
        info.quaGameStatus == PHASE::RUNNING)
	{
        info.quaGameStatus = PHASE::OVER;
	}
    if (info.quaGameStatus == PHASE::RUNNING && info.binShootout == SHOOTOUT::YES)
	{
        ++info.nTimeByRounds;
        if (info.nTimeByRounds / nFPS == 20)
		{
            info.quaGameStatus = PHASE::OVER;
            info.binShootout = SHOOTOUT::NO;
		}
	}
}

void MainLogic::ShootOut(int nSide)
{
    info.binShootout = SHOOTOUT::YES;
    info.binSideShoot = nSide;
    info.quaGameStatus = PHASE::RUNNING;
    info.nTimeByRounds = 0;
}

void MainLogic::PlusOne(int nSide)
{
	if (nSide == SIDE::SIDE_A)
	{
        ++info.nScoreA;
	}
	if (nSide == SIDE::SIDE_B)
	{
        ++info.nScoreB;
	}
}

void MainLogic::Penalty(int nSide)
{
	if (nSide == SIDE::SIDE_A)
	{
        info.nEvilA += thePenalty;
	}
	if (nSide == SIDE::SIDE_B)
	{
        info.nEvilB += thePenalty;
	}
}

MatchInfo MainLogic::GetInfo()
{
    return info;
}

