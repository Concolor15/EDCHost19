#include "stdafx.h"
#include "config.h"
#include "MatchMain.h"
#include "EDCHost19.h"

MatchMain::MatchMain(QWidget *parent)
    : QDialog(parent), logic(new MainLogic)
{
	ui.setupUi(this);
	QObject::connect(dynamic_cast<EDCHost19*>(parent), &EDCHost19::PassInfo, this, &MatchMain::Running);
    logic->ResetInfo();
}

MatchMain::~MatchMain()
{
    delete logic;
}

void MatchMain::paintEvent(QPaintEvent * event)
{
    auto infoMatch = logic->GetInfo();
    auto infoPhase = infoMatch.quaGameStatus;
    bool bInHalt[2] = {
        infoMatch.nHaltRound[0] != 0,
        infoMatch.nHaltRound[1] != 0,
    };
	int countDown;
    if (infoMatch.binShootout == NO)
	{
        countDown = GAME_TIME - infoMatch.nTimeByRounds / nFPS;
	}
	else
	{
        countDown = 20 - infoMatch.nTimeByRounds / nFPS;
	}
	ui.btnBeginEnd->setDisabled(false);
	ui.btnPauseResume->setDisabled(infoPhase != PHASE::RUNNING && infoPhase != PHASE::PAUSE);
    ui.btnPenaltyA->setDisabled(bInHalt[0] || infoPhase != PHASE::RUNNING);
    ui.btnPenaltyB->setDisabled(bInHalt[1] || infoPhase != PHASE::RUNNING);
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
        ui.lblEAVal->setText(QString("%1").arg(infoMatch.nEvil[0], 3, 10, QChar('0')));
        ui.lblEBVal->setText(QString("%1").arg(infoMatch.nEvil[1], 3, 10, QChar('0')));
        ui.lblHAVal->setText(QString("%1").arg(infoMatch.nHalt[0], 1, 10, QChar('0')));
        ui.lblHBVal->setText(QString("%1").arg(infoMatch.nHalt[1], 1, 10, QChar('0')));
        ui.lblSAVal->setText(QString("%1").arg(infoMatch.nScore[0], 2, 10, QChar('0')));
        ui.lblSBVal->setText(QString("%1").arg(infoMatch.nScore[1], 2, 10, QChar('0')));
		ui.lblTVal->setText(QString("%1").arg(countDown, 3, 10, QChar('0')));
		break;
	case PHASE::PAUSE:
		ui.btnBeginEnd->setText("结束");
		ui.btnPauseResume->setText("继续");
        ui.lblEAVal->setText(QString("%1").arg(infoMatch.nEvil[0], 3, 10, QChar('0')));
        ui.lblEBVal->setText(QString("%1").arg(infoMatch.nEvil[1], 3, 10, QChar('0')));
        ui.lblHAVal->setText(QString("%1").arg(infoMatch.nHalt[0], 1, 10, QChar('0')));
        ui.lblHBVal->setText(QString("%1").arg(infoMatch.nHalt[1], 1, 10, QChar('0')));
        ui.lblSAVal->setText(QString("%1").arg(infoMatch.nScore[0], 2, 10, QChar('0')));
        ui.lblSBVal->setText(QString("%1").arg(infoMatch.nScore[1], 2, 10, QChar('0')));
		ui.lblTVal->setText(QString("%1").arg(countDown, 3, 10, QChar('0')));
		break;
	default:
		break;
	}
	QDialog::paintEvent(event);
}

void MatchMain::closeEvent(QCloseEvent * event)
{
	if (QMessageBox::question(this, "提示", "请确认是否退出？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		deleteLater();
		QDialog::closeEvent(event);
	}
	else
	{
		event->ignore();
	}
}

void MatchMain::OnPR()
{
    auto info = logic->GetInfo().quaGameStatus;
	if (info == PHASE::RUNNING)
	{
        logic->MatchPause();
	}
	if (info == PHASE::PAUSE)
	{
        logic->MatchResume();
	}
}

void MatchMain::OnBE()
{
    auto info = logic->GetInfo().quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
        logic->MatchEnd();
	}
	if (info == PHASE::NOTBEGIN || info == PHASE::OVER)
	{
        logic->MatchBegin();
	}
}

void MatchMain::OnSOA()
{
    auto info = logic->GetInfo().quaGameStatus;
	if (info == PHASE::NOTBEGIN || info == PHASE::OVER)
	{
        logic->ShootOut(SIDE_A);
	}
}

void MatchMain::OnSOB()
{
    auto info = logic->GetInfo().quaGameStatus;
	if (info == PHASE::NOTBEGIN || info == PHASE::OVER)
	{
        logic->ShootOut(SIDE_B);
	}
}

void MatchMain::OnPlusA()
{
    auto info = logic->GetInfo().quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
        logic->PlusOne(SIDE_A);
	}
}

void MatchMain::OnPlusB()
{
    auto info = logic->GetInfo().quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
        logic->PlusOne(SIDE_B);
	}
}

void MatchMain::OnPenaltyA()
{
    auto info = logic->GetInfo().quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
        logic->Penalty(SIDE_A);
	}
}

void MatchMain::OnPenaltyB()
{
    auto info = logic->GetInfo().quaGameStatus;
	if (info == PHASE::RUNNING || info == PHASE::PAUSE)
	{
        logic->Penalty(SIDE_B);
	}
}

void MatchMain::Running(CameraInfo infoReady,QPixmap pixShow)
{
    logic->Run(infoReady);
	ui.lblCamera->setPixmap(pixShow);
    Serial::GetInstance()->Transmit(logic->GetInfo());
}
