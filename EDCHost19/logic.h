#ifndef LOGIC_H
#define LOGIC_H

#include <QtCore>
#include "locate.h"

class Logic: public QObject
{
    Q_OBJECT

    Q_PROPERTY(State status    READ getStatus      NOTIFY statusChanged)
    Q_PROPERTY(int elapsedTime READ getElapsedTime NOTIFY elapsedTimeChanged)

    Q_PROPERTY(QPointF rawBallPos READ getRawBallPos  NOTIFY rawBallPosChanged)
    Q_PROPERTY(QPointF rawCarAPos READ getRawCarAPos  NOTIFY rawCarAPosChanged)
    Q_PROPERTY(QPointF rawCarBPos READ getRawCarBPos  NOTIFY rawCarBPosChanged)

public:
    enum State {
        NotStart,
        Running,
        Paused,
        Finished
    };
    Q_ENUM(State)

    Logic(QObject* parent = Q_NULLPTR);

    void packToByteArray(uint8_t (&data)[32]);
    void run(LocateResult const* info);

    State getStatus()    const { return m_status; }
    int getElapsedTime() const { return m_elapsedTime; }

    QPointF getRawBallPos() const { return m_ball.raw_center; }
    QPointF getRawCarAPos() const { return m_car[0].raw_center; }
    QPointF getRawCarBPos() const { return m_car[1].raw_center; }

    bool getBallLocated() const { return m_ball.located; }
    bool getCarALocated() const { return m_car[0].located; }
    bool getCarBLocated() const { return m_car[1].located; }

signals:
    void elapsedTimeChanged(int newElapsedTime);
    void statusChanged(State newStatus);

    void rawBallPosChanged(QPointF newBallPos);
    void rawCarAPosChanged(QPointF newCarAPos);
    void rawCarBPosChanged(QPointF newCarBPos);
public slots:
    void start();
    void stop(bool runOutTime=false);
    void pause();
    void resume();

private:
    void updateAll();
    void reset();

    int m_elapsedTime;
    State m_status;

    ObjectTracker::Report m_ball;
    ObjectTracker::Report m_car[2];

    static constexpr double LogicWidth = 297;
    static constexpr double LogicHeight = 210;
    static constexpr int TotalTime = 360000;
};

#endif // LOGIC_H
