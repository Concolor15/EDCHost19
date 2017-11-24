#ifndef LOGIC_H
#define LOGIC_H

#include <QtCore>
#include "type.h"

class Logic: public QObject
{
    Q_OBJECT

    Q_PROPERTY(State status    READ getStatus      NOTIFY statusChanged)
    Q_PROPERTY(int elapsedTime READ getElapsedTime NOTIFY elapsedTimeChanged)

    Q_PROPERTY(QPointF ballPos READ getBallPos  NOTIFY ballPosChanged)
    Q_PROPERTY(QPointF carAPos READ getCarAPos  NOTIFY carAPosChanged)
    Q_PROPERTY(QPointF carBPos READ getCarBPos  NOTIFY carBPosChanged)

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

    QPointF getBallPos() const { return m_ball; }
    QPointF getCarAPos() const { return m_cars[0]; }
    QPointF getCarBPos() const { return m_cars[1]; }

signals:
    void elapsedTimeChanged(int newElapsedTime);
    void statusChanged(State newStatus);

    void ballPosChanged(QPointF newBallPos);
    void carAPosChanged(QPointF newCarAPos);
    void carBPosChanged(QPointF newCarBPos);
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

    QPointF m_ball;
    QPointF m_cars[2];

    static constexpr double LogicWidth = 297;
    static constexpr double LogicHeight = 210;
    static constexpr int TotalTime = 360000;
};

#endif // LOGIC_H
