#include "stdafx.h"
#include "MatchMain.h"
#include "EDCHost19.h"

MatchMain::MatchMain(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	QObject::connect(dynamic_cast<EDCHost19*>(parent), &EDCHost19::PassInfo, this, &MatchMain::Running);
}

MatchMain::~MatchMain()
{
	
}

void MatchMain::paintEvent(QPaintEvent * event)
{
	auto infoMatch = MainLogic::GetInstance()->GetInfo();
	auto infoPhase = infoMatch.infoObjs.quaGameStatus;
	bool bAInHalt = infoMatch.infoObjs.nHaltRoundA != 0;
	bool bBInHalt = infoMatch.infoObjs.nHaltRoundB != 0;
	int countDown;
	if (infoMatch.infoObjs.binShootout == NO)
	{
		countDown = 360 - infoMatch.infoObjs.nTimeByRounds / nFPS;
	}
	else
	{
		countDown = 20 - infoMatch.infoObjs.nTimeByRounds / nFPS;
	}
	ui.btnBeginEnd->setDisabled(false);
	ui.btnPauseResume->setDisabled(infoPhase != PHASE::RUNNING && infoPhase != PHASE::PAUSE);
	ui.btnPenaltyA->setDisabled(bAInHalt || infoPhase != PHASE::RUNNING);
	ui.btnPenaltyB->setDisabled(bBInHalt || infoPhase != PHASE::RUNNING);
	ui.btnPlusA->setDisabled(infoPhase != PHASE::RUNNING);
	ui.btnPlusB->setDisabled(infoPhase != PHASE::RUNNING);
	ui.btnSOA->setDisabled(infoPhase != PHASE::OVER && infoPhase != PHASE::NOTBEGIN);
	ui.btnSOB->setDisabled(infoPhase != PHASE::OVER && infoPhase != PHASE::NOTBEGIN);
	switch (infoPhase)
	{
	case PHASE::NOTBEGIN:
	case PHASE::OVER:
		ui.btnBeginEnd->setText("开始");
		ui.btnPauseResume->setText("暂停/继续");
		ui.lblEAVal->setText("000");
		ui.lblEBVal->setText("000");
		ui.lblHAVal->setText("0");
		ui.lblHBVal->setText("0");
		ui.lblSAVal->setText("00");
		ui.lblSBVal->setText("00");
		ui.lblTVal->setText(QString("%1").arg(countDown,3,10,QChar('0')));
		break;
	case PHASE::RUNNING:
		ui.btnBeginEnd->setText("结束");
		ui.btnPauseResume->setText("暂停");
		ui.lblEAVal->setText(QString("%1").arg(infoMatch.infoObjs.nEvilA, 3, 10, QChar('0')));
		ui.lblEBVal->setText(QString("%1").arg(infoMatch.infoObjs.nEvilB, 3, 10, QChar('0')));
		ui.lblHAVal->setText(QString("%1").arg(infoMatch.nHaltA, 1, 10, QChar('0')));
		ui.lblHBVal->setText(QString("%1").arg(infoMatch.nHaltB, 1, 10, QChar('0')));
		ui.lblSAVal->setText(QString("%1").arg(infoMatch.infoObjs.nScoreA, 2, 10, QChar('0')));
		ui.lblSBVal->setText(QString("%1").arg(infoMatch.infoObjs.nScoreB, 2, 10, QChar('0')));
		ui.lblTVal->setText(QString("%1").arg(countDown, 3, 10, QChar('0')));
		break;
	case PHASE::PAUSE:
		ui.btnBeginEnd->setText("结束");
		ui.btnPauseResume->setText("继续");
		ui.lblEAVal->setText(QString("%1").arg(infoMatch.infoObjs.nEvilA, 3, 10, QChar('0')));
		ui.lblEBVal->setText(QString("%1").arg(infoMatch.infoObjs.nEvilB, 3, 10, QChar('0')));
		ui.lblHAVal->setText(QString("%1").arg(infoMatch.nHaltA, 1, 10, QChar('0')));
		ui.lblHBVal->setText(QString("%1").arg(infoMatch.nHaltB, 1, 10, QChar('0')));
		ui.lblSAVal->setText(QString("%1").arg(infoMatch.infoObjs.nScoreA, 2, 10, QChar('0')));
		ui.lblSBVal->setText(QString("%1").arg(infoMatch.infoObjs.nScoreB, 2, 10, QChar('0')));
		ui.lblTVal->setText(QString("%1").arg(countDown, 3, 10, QChar('0')));
		break;
	default:
		break;
	}
	QDialog::paintEvent(event);
}

void MatchMain::closeEvent(QCloseEvent * event)
{
	auto infoMatch = MainLogic::GetInstance()->GetInfo();
	if (infoMatch.infoObjs.quaGameStatus == PHASE::RUNNING || infoMatch.infoObjs.quaGameStatus == PHASE::PAUSE)
	{
		if (QMessageBox::information(this, "提示", "当前比赛尚未结束，请问是否退出？") == QMessageBox::No)
		{
			event->ignore();
		}
		else
		{
			parentWidget()->show();
		}
	}
	else
	{
		parentWidget()->show();
	}
	QDialog::closeEvent(event);
}

void MatchMain::OnPR()
{
	auto info = MainLogic::GetInstance()->GetInfo().infoObjs.quaGameStatus;
	if (info == PHASE::RUNNING)
	{
		MainLogic::GetInstance()->MatchPause();
	}
	if (info == PHASE::PAUSE)
	{
		MainLogic::GetInstance()->MatchResume();
	}
}

void MatchMain::OnBE()
{
	auto info = MainLogic::GetInstance()->GetInfo().infoObjs.quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
		MainLogic::GetInstance()->MatchEnd();
	}
	if (info == PHASE::NOTBEGIN || info == PHASE::OVER)
	{
		MainLogic::GetInstance()->MatchBegin();
	}
}

void MatchMain::OnSOA()
{
	auto info = MainLogic::GetInstance()->GetInfo().infoObjs.quaGameStatus;
	if (info == PHASE::NOTBEGIN || info == PHASE::OVER)
	{
		MainLogic::GetInstance()->ShootOut(SIDE_A);
	}
}

void MatchMain::OnSOB()
{
	auto info = MainLogic::GetInstance()->GetInfo().infoObjs.quaGameStatus;
	if (info == PHASE::NOTBEGIN || info == PHASE::OVER)
	{
		MainLogic::GetInstance()->ShootOut(SIDE_B);
	}
}

void MatchMain::OnPlusA()
{
	auto info = MainLogic::GetInstance()->GetInfo().infoObjs.quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
		MainLogic::GetInstance()->PlusOne(SIDE_A);
	}
}

void MatchMain::OnPlusB()
{
	auto info = MainLogic::GetInstance()->GetInfo().infoObjs.quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
		MainLogic::GetInstance()->PlusOne(SIDE_B);
	}
}

void MatchMain::OnPenaltyA()
{
	auto info = MainLogic::GetInstance()->GetInfo().infoObjs.quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
		MainLogic::GetInstance()->Penalty(SIDE_A);
	}
}

void MatchMain::OnPenaltyB()
{
	auto info = MainLogic::GetInstance()->GetInfo().infoObjs.quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
		MainLogic::GetInstance()->Penalty(SIDE_B);
	}
}

void MatchMain::Running(CameraInfo infoReady,QPixmap pixShow)
{
	MainLogic::GetInstance()->Run(infoReady);
	ui.lblCamera->setPixmap(pixShow);
	Serial::GetInstance()->Transmit(MainLogic::GetInstance()->GetInfo().infoObjs);
}
