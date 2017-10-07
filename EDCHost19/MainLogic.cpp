#include "stdafx.h"
#include "MainLogic.h"

constexpr int thePenalty = 50;

MainLogic * MainLogic::pInstance = nullptr;
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
	theMatch.nAInBRounds = 0;
	theMatch.nBInARounds = 0;
	theMatch.infoObjs.binShootout = SHOOTOUT::NO;
	theMatch.infoObjs.binSideShoot = SIDE::SIDE_B;
	theMatch.infoObjs.nEvilA = 0;
	theMatch.infoObjs.nEvilB = 0;
	theMatch.infoObjs.nHaltRoundA = 0;
	theMatch.infoObjs.nHaltRoundB = 0;
	theMatch.infoObjs.nScoreA = 0;
	theMatch.infoObjs.nScoreB = 0;
	theMatch.infoObjs.nTimeByRounds = 0;
	theMatch.infoObjs.posObjs = CameraInfo();
	theMatch.infoObjs.quaGameStatus = PHASE::NOTBEGIN;
	theMatch.nHaltA = 0;
	theMatch.nHaltB = 0;
}

void MainLogic::EvaluateEvil()
{
	//Running to calculate
	if (theMatch.infoObjs.quaGameStatus != PHASE::RUNNING) return;
	if (theMatch.infoObjs.binShootout == SHOOTOUT::YES) return;
	const auto &posA = theMatch.infoObjs.posObjs.posCarA;
	const auto &posB = theMatch.infoObjs.posObjs.posCarB;
	if (posA.second > theMatch.GetHalfLength() && theMatch.infoObjs.nHaltRoundA != 0)
	{
		++theMatch.nAInBRounds;
		if (theMatch.nAInBRounds % nFPS == 0 && theMatch.infoObjs.nHaltRoundA != 0)
		{
			auto d = theMatch.GetLength() - posA.second;
			theMatch.infoObjs.nEvilA += 5 - d / 30;
		}
	}
	if (posB.second <= theMatch.GetHalfLength() && theMatch.infoObjs.nHaltRoundB != 0)
	{
		++theMatch.nBInARounds;
		if (theMatch.nBInARounds % nFPS == 0)
		{
			auto d = posB.second - 0;
			theMatch.infoObjs.nEvilB += 5 - d / 30;
		}
	}
}

void MainLogic::ResetEvil(int nSide)
{
	if (nSide == SIDE::SIDE_A)
	{
		theMatch.nAInBRounds = 0;
		theMatch.infoObjs.nEvilA = 0;
	}
	if (nSide == SIDE::SIDE_B)
	{
		theMatch.nBInARounds = 0;
		theMatch.infoObjs.nEvilB = 0;
	}
}

MainLogic * MainLogic::GetInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new MainLogic;
	}
	return pInstance;
}

void MainLogic::DestroyInstance()
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

void MainLogic::MatchBegin()
{
	ResetInfo();
	theMatch.infoObjs.quaGameStatus = PHASE::RUNNING;
}

void MainLogic::MatchEnd()
{
	theMatch.infoObjs.quaGameStatus = PHASE::OVER;
	ResetInfo();
}

void MainLogic::MatchPause()
{
	theMatch.infoObjs.quaGameStatus = PHASE::PAUSE;
}

void MainLogic::MatchResume()
{
	theMatch.infoObjs.quaGameStatus = PHASE::RUNNING;
}

void MainLogic::Run(const CameraInfo & pts)
{
	//Update position info
	theMatch.infoObjs.posObjs = pts;
	//Update Evil
	EvaluateEvil();
	//Deal with penalty of halting
	if (theMatch.infoObjs.binShootout == SHOOTOUT::NO && theMatch.infoObjs.quaGameStatus == PHASE::RUNNING)
	{
		++theMatch.infoObjs.nTimeByRounds;
		if (theMatch.infoObjs.nHaltRoundA == 0)
		{
			if (theMatch.infoObjs.nEvilA >= 100)
			{
				auto nStopSecA = MAX((5 + 2 * theMatch.nHaltA), 15);
				theMatch.infoObjs.nHaltRoundA = nStopSecA * nFPS;
				++theMatch.nHaltA;
			}
		}
		else
		{
			if (theMatch.infoObjs.nHaltRoundA == 1)
			{
				ResetEvil(SIDE_A);
			}
			--theMatch.infoObjs.nHaltRoundA;
		}
		if (theMatch.infoObjs.nHaltRoundB == 0)
		{
			if (theMatch.infoObjs.nEvilB >= 100)
			{
				auto nStopSecB = MAX((5 + 2 * theMatch.nHaltB), 15);
				theMatch.infoObjs.nHaltRoundB = nStopSecB * nFPS;
				++theMatch.nHaltB;
			}
		}
		else
		{
			if (theMatch.infoObjs.nHaltRoundB == 1)
			{
				ResetEvil(SIDE_B);
			}
			--theMatch.infoObjs.nHaltRoundB;
		}
	}
	//Time up
	if (theMatch.infoObjs.nTimeByRounds / nFPS == 360 && 
		theMatch.infoObjs.binShootout == SHOOTOUT::NO &&
		theMatch.infoObjs.quaGameStatus == PHASE::RUNNING)
	{
		theMatch.infoObjs.quaGameStatus = PHASE::OVER;
	}
	if (theMatch.infoObjs.quaGameStatus == PHASE::RUNNING && theMatch.infoObjs.binShootout == SHOOTOUT::YES)
	{
		++theMatch.infoObjs.nTimeByRounds;
		if (theMatch.infoObjs.nTimeByRounds / nFPS == 20)
		{
			theMatch.infoObjs.quaGameStatus = PHASE::OVER;
			theMatch.infoObjs.binShootout = SHOOTOUT::NO;
		}
	}
}

void MainLogic::ShootOut(int nSide)
{
	theMatch.infoObjs.binShootout = SHOOTOUT::YES;
	theMatch.infoObjs.binSideShoot = nSide;
	theMatch.infoObjs.quaGameStatus = PHASE::RUNNING;
	theMatch.infoObjs.nTimeByRounds = 0;
}

void MainLogic::PlusOne(int nSide)
{
	if (nSide == SIDE::SIDE_A)
	{
		++theMatch.infoObjs.nScoreA;
	}
	if (nSide == SIDE::SIDE_B)
	{
		++theMatch.infoObjs.nScoreB;
	}
}

void MainLogic::Penalty(int nSide)
{
	if (nSide == SIDE::SIDE_A)
	{
		theMatch.infoObjs.nEvilA += thePenalty;
	}
	if (nSide == SIDE::SIDE_B)
	{
		theMatch.infoObjs.nEvilB += thePenalty;
	}
}

MatchInfo MainLogic::GetInfo()
{
	return theMatch;
}

