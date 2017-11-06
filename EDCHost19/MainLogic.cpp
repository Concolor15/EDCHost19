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
    info.nInOpRound[0] = 0;
    info.nInOpRound[1] = 0;
    info.binShootout = SHOOTOUT::NO;
    info.binSideShoot = SIDE::SIDE_B;
    info.nEvil[0] = 0;
    info.nEvil[1] = 0;
    info.nHaltRound[0] = 0;
    info.nHaltRound[1] = 0;
    info.nScore[0] = 0;
    info.nScore[1] = 0;
    info.nTimeByRounds = 0;
    info.posObjs = CameraInfo();
    info.quaGameStatus = PHASE::NOTBEGIN;
    info.nHalt[0] = 0;
    info.nHalt[1] = 0;
}

void MainLogic::EvaluateEvil()
{
	//Running to calculate
    if (info.quaGameStatus != PHASE::RUNNING) return;
    if (info.binShootout == SHOOTOUT::YES) return;
    QPoint pos[2] = {info.posObjs.posCar1, info.posObjs.posCar2};
    for (int side : {0, 1})
    {
        if (pos[side].y() > info.HALF_LENGTH && info.nHaltRound[side] !=0 )
        {
            ++info.nInOpRound[side];
            if (info.nInOpRound[side] % nFPS == 0 && info.nHaltRound[side] !=0)
            {
                auto d = info.LENGTH - pos[side].y();
                info.nEvil[side] += 5-d/30;
            }
        }
    }
}

void MainLogic::ResetEvil(int side)
{
    info.nInOpRound[side] = 0;
    info.nEvil[side] = 0;
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
        for (int side : {0, 1})
        {
            if (info.nHaltRound[side] == 0)
            {
                if (info.nEvil[side] >= 100)
                {
                    auto nStopSec = MAX((5 + 2 * info.nHalt[side]), 15);
                    info.nHaltRound[side] = nStopSec * nFPS;
                    ++info.nHalt[0];

                }
            }
            else
            {
                if (info.nHaltRound[side] == 1)
                {
                    ResetEvil(side);
                }
                --info.nHaltRound[side];
            }
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

void MainLogic::ShootOut(int side)
{
    info.binShootout = SHOOTOUT::YES;
    info.binSideShoot = side;
    info.quaGameStatus = PHASE::RUNNING;
    info.nTimeByRounds = 0;
}

void MainLogic::PlusOne(int side)
{
    ++info.nScore[side];
}

void MainLogic::Penalty(int side)
{
    info.nEvil[side] += thePenalty;
}

MatchInfo MainLogic::GetInfo()
{
    return info;
}

