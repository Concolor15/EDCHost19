#pragma once

#include <QtCore>
#include <QObject>
#include <QCamera>
#include "Camera.h"
#include "GlobalType.h"

class MainLogic: public QObject
{
    Q_OBJECT
    Q_PROPERTY(PHASE status READ status NOTIFY statusChanged)
    Q_PROPERTY(int time READ time NOTIFY timeChanged)
    Q_PROPERTY(int shootSide READ shootSide NOTIFY shootSideChanged)
    Q_PROPERTY(bool isShoot READ isShoot NOTIFY isShootChanged)
private:
    MatchInfo info;
public:
    MainLogic(QObject* parent=nullptr);
    ~MainLogic() = default;

    PHASE status()  const {return info.quaGameStatus; }
    int time()      const {return info.nTimeByRounds; }
    int shootSide() const {return info.shootSide; }
    bool isShoot()  const {return info.binShootout; }

    Q_INVOKABLE void MatchBegin();
    Q_INVOKABLE void MatchEnd();
    Q_INVOKABLE void MatchPause();
    Q_INVOKABLE void MatchResume();
    Q_INVOKABLE void PlusOne(int nSide);
    Q_INVOKABLE void Penalize(int nSide);
    Q_INVOKABLE void ShootOut(int nSide);
    Q_INVOKABLE void ResetEvil(int nSide);

	void Run(const CameraInfo &pts);

	void ResetInfo();
	void EvaluateEvil();

    MatchInfo const& GetInfo();

signals:
    void statusChanged(PHASE newStatus);
    void timeChanged(int newTime);
    void shootSideChanged(int newShootSide);
    void isShootChanged(bool newIsShoot);
};

