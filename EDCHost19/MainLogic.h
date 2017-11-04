#pragma once

#include "GlobalType.h"

class MainLogic final
{
private:
	MatchInfo theMatch;
public:
    MainLogic();
    ~MainLogic();
	void MatchBegin();
	void MatchEnd();
	void MatchPause();
	void MatchResume();
	void Run(const CameraInfo &pts);
	void ShootOut(int nSide);
	void PlusOne(int nSide);
	void Penalty(int nSide);
	void ResetInfo();
	void EvaluateEvil();
	void ResetEvil(int nSide);
	MatchInfo GetInfo();
};
