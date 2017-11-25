#ifndef LOGIC_H
#define LOGIC_H

#include <QtCore>
#include "locate.h"

class Logic: public QObject
{
    Q_OBJECT

    Q_PROPERTY(State status    READ getStatus      NOTIFY statusChanged)
    Q_PROPERTY(int elapsedTime READ getElapsedTime NOTIFY elapsedTimeChanged)  
    Q_PROPERTY(int shootSide   READ getShootSide   NOTIFY shootSideChanged)

    Q_PROPERTY(bool shouldStopA READ getShouldStopA NOTIFY shouldStopAChanged)
    Q_PROPERTY(bool shouldStopB READ getShouldStopB NOTIFY shouldStopBChanged)

    Q_PROPERTY(int restStopA READ getRestStopA NOTIFY restStopAChanged)
    Q_PROPERTY(int restStopB READ getRestStopB NOTIFY restStopBChanged)

    Q_PROPERTY(QPointF rawBallPos READ getRawBallPos  NOTIFY rawBallPosChanged)
    Q_PROPERTY(QPointF rawCarAPos READ getRawCarAPos  NOTIFY rawCarAPosChanged)
    Q_PROPERTY(QPointF rawCarBPos READ getRawCarBPos  NOTIFY rawCarBPosChanged)

    Q_PROPERTY(int scoreA READ getScoreA NOTIFY scoreAChanged)
    Q_PROPERTY(int scoreB READ getScoreB NOTIFY scoreBChanged)
    Q_PROPERTY(int evilA  READ getEvilA  NOTIFY evilAChanged)
    Q_PROPERTY(int evilB  READ getEvilB  NOTIFY evilBChanged)

public:
    enum State {
        NotStart,
        Running,
        Paused,
        Finished
    };
    Q_ENUM(State)

    explicit Logic(QObject* parent = Q_NULLPTR);

    void packToByteArray(uint8_t (&data)[32]);
    void run(LocateResult const* info);

    State getStatus()    const { return m_status; }
    int getElapsedTime() const { return m_elapsedTime; }
    int getShootSide() const {return m_shootSide; }

    int getShouldStopA() const {return m_elapsedTime < m_stopUntil[0]; }
    int getShouldStopB() const {return m_elapsedTime < m_stopUntil[1]; }

    int getRestStopA() const {return std::max(0, m_stopUntil[0]-m_elapsedTime); }
    int getRestStopB() const {return std::max(0, m_stopUntil[1]-m_elapsedTime); }

    int getScoreA() const {return m_score[0]; }
    int getScoreB() const {return m_score[1]; }
    int getEvilA()  const {return m_evil[0]; }
    int getEvilB()  const {return m_evil[1]; }

    QPointF getRawBallPos() const { return m_ball.raw_center; }
    QPointF getRawCarAPos() const { return m_car[0].raw_center; }
    QPointF getRawCarBPos() const { return m_car[1].raw_center; }

    bool getBallLocated() const { return m_ball.located; }
    bool getCarALocated() const { return m_car[0].located; }
    bool getCarBLocated() const { return m_car[1].located; }

signals:
    void elapsedTimeChanged(int newElapsedTime);
    void statusChanged(State newStatus);
    void shootSideChanged(int newShootSide);

    void scoreAChanged(int newScoreA);
    void scoreBChanged(int newScoreB);
    void evilAChanged(int newEvilA);
    void evilBChanged(int newEvilB);

    void shouldStopAChanged(bool newShouldStopA);
    void shouldStopBChanged(bool newShouldStopB);
    void restStopAChanged(bool newRestStopA);
    void restStopBChanged(bool newRestStopB);

    void rawBallPosChanged(QPointF newBallPos);
    void rawCarAPosChanged(QPointF newCarAPos);
    void rawCarBPosChanged(QPointF newCarBPos);
public slots:
    void start();
    void stop(bool runOutTime=false);
    void pause();
    void resume();

    void setSide(int side);
    void setScore(int side, int score);

private:
    void updateAll();
    void reset_start();

    int m_elapsedTime = 0;
    State m_status = NotStart;
    int m_shootSide = 0;

    int m_stopUntil[2] = {};

    int m_score[2] = {};
    int m_evil[2] = {};

    ObjectTracker::Report m_ball = {};
    ObjectTracker::Report m_car[2] = {};

    static constexpr double LogicWidth = 297;
    static constexpr double LogicHeight = 210;
    static constexpr int TotalTime = 360000;
};

#endif // LOGIC_H
