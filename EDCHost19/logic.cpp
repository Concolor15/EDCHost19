#include "stdafx.h"
#include "logic.h"

#include "globalconfig.h"
#include <cmath>

using namespace std;
Logic::Logic(QObject* parent) : QObject(parent)
{

}

void Logic::start()
{
    Q_ASSERT(m_status == NotStart || m_status == Finished);

    reset_start();
    m_status = Running;
    updateAll();
}

void Logic::stop(bool RunOutTime)
{
    Q_UNUSED(RunOutTime)
    Q_ASSERT(m_status == Running || m_status == Paused);

    m_status = Finished;
    emit statusChanged(m_status);
}

void Logic::pause()
{
    Q_ASSERT(m_status == Running);

    m_status = Paused;
    emit statusChanged(m_status);
}

void Logic::resume()
{
    Q_ASSERT(m_status == Paused);

    m_status = Running;
    emit statusChanged(m_status);
}

void Logic::setSide(int side)
{
    if (side!=0 && side !=1)
    {
        qCritical() << "setScore: side must be 0 or 1, but get " << side << "\n";
        return;
    }

    if (m_status == Running || m_status == Paused)
    {
        qWarning() << "setScore: status should net be " << m_status;
        return;
    }

    if (m_shootSide == side)
        return;

    m_shootSide = side;
    emit shootSideChanged(side);
}

void Logic::setScore(int side, int score)
{
    if (side!=0 && side !=1)
    {
        qCritical() << "setScore: side must be 0 or 1, but get " << side;
        return;
    }

    if (score<0)
    {
        qWarning() << "setScore: score must be positive, but get " << score;
        return;
    }

    if (m_score[side] == score)
        return;

    m_score[side] = score;

    if (side==0)
        emit scoreAChanged(score);
    else
        emit scoreBChanged(score);
}

void Logic::updateAll()
{
    emit statusChanged(m_status);
    emit elapsedTimeChanged(m_elapsedTime);
    emit shootSideChanged(m_shootSide);
    emit scoreAChanged(m_score[0]);
    emit scoreBChanged(m_score[1]);
    emit evilAChanged(m_evil[0]);
    emit evilBChanged(m_evil[1]);
    //emit shouldStopAChanged(getShouldStopA());
    //emit shouldStopBChanged(getShouldStopB());
    emit restStopAChanged(getRestStopA());
    emit restStopBChanged(getRestStopB());
    emit rawBallPosChanged(m_ball.raw_center);
    emit rawCarAPosChanged(m_car[0].raw_center);
    emit rawCarBPosChanged(m_car[1].raw_center);
}

void Logic::reset_start()
{
    m_status = NotStart;
    m_elapsedTime = 0;
    m_shoot_side_protected = 0;
    m_stopCount[0] = 0;
    m_stopCount[1] = 0;
    m_restStop[0] = 0;
    m_restStop[1] = 0;
    m_evil[0] = 0;
    m_evil[1] = 0;
    m_ball = {};
    m_car[0] = {};
    m_car[1] = {};

    updateAll();
}

void Logic::setEvil(int side, double newEvil)
{
    Q_ASSERT(side ==0 || side == 1);
    Q_ASSERT(newEvil >= 0);

    if (newEvil == m_evil[side])
        return;

    m_evil[side] = newEvil;

    if (side==0)
        emit evilAChanged(getEvilA());
    else
        emit evilBChanged(getEvilB());
}

void Logic::setRestStop(int side, int newRestStop)
{
    Q_ASSERT(side ==0 || side == 1);
    Q_ASSERT(newRestStop >= 0);

    if (newRestStop == m_restStop[side])
        return;

    m_restStop[side] = newRestStop;

    if (side==0)
        emit restStopAChanged(newRestStop);
    else
        emit restStopBChanged(newRestStop);
}

void Logic::reduceRestStop(int side)
{
    Q_ASSERT(side ==0 || side == 1);

    if (m_restStop[side]>0)
    {
        m_restStop[side] -= 1;

        if (side==0)
            emit restStopAChanged(m_restStop[0]);
        else
            emit restStopBChanged(m_restStop[1]);
    }
}


static void writePoint(uint8_t* addr, ObjectTracker::Report const& data)
{
    int x = (int)data.center.x();
    int y = (int)data.center.y();
    bool located = data.located;
    addr[0] = (uint8_t)y;
    addr[1] = (x >> 8) | (located ? 0 : 0x80);
    addr[2] = x & 0xFF;
}

void Logic::packToByteArray(uint8_t (&data)[32])
{
    for (auto& x : data) x=0;

    data[0] = 0xFE | (m_shootSide^1);
    data[1] = ((m_elapsedTime >> 8) & 0x3F) | 0x40;
    data[2] = m_elapsedTime & 0xFF;

    writePoint(&data[3], m_car[0]);
    writePoint(&data[6], m_car[1]);
    writePoint(&data[9], m_ball);

    int stopRestA = getRestStopA();
    int stopRestB = getRestStopB();

    data[12] = stopRestA >> 8;
    data[13] = stopRestA & 0xFF;
    data[14] = stopRestB >> 8;
    data[15] = stopRestB & 0xFF;
    data[16] = m_evil[0];
    data[17] = m_evil[1];
    data[18] = m_score[0];
    data[19] = m_score[1];
    data[20] = m_status << 6;

    if (m_status != Running)
    {
        data[12] |= 0x80;
        data[14] |= 0x80;
    }

    //uint16_t checksum = qChecksum((const char*)data, 28);
    //data[28] = checksum >> 8;
    //data[29] = checksum & 0xFF;
    uint8_t checksum = 0;
    for (int i=0;i<28;i++) checksum ^= data[i];
    data[29] = checksum;

    data[30] = 0x0D;
    data[31] = 0x0A;

    /*
    data_buffer[0] = 0xFC | (data.binShootout << 1) | (data.shootSide^1);
    data_buffer[1] = (data.quaGameStatus << 6) | (data.nTimeByRounds >> 8);
    data_buffer[2] = data.nTimeByRounds & 0xFF;

    data_buffer[3] = pos.posCar1.x();
    data_buffer[4] = pos.posCar1.y() >> 8;
    data_buffer[5] = pos.posCar1.y() & 0xFF;

    data_buffer[6] = pos.posCar2.x();
    data_buffer[7] = pos.posCar2.y() >> 8;
    data_buffer[8] = pos.posCar2.y() & 0xFF;

    data_buffer[9] = pos.posBall.x();
    data_buffer[10] = pos.posBall.y() >> 8;
    data_buffer[11] = pos.posBall.y() & 0xFF;

    data_buffer[12] = data.nHaltRound[0] >> 8;
    data_buffer[13] = data.nHaltRound[0] & 0xFF;
    data_buffer[14] = data.nHaltRound[1] >> 8;
    data_buffer[15] = data.nHaltRound[1] & 0xFF;
    data_buffer[16] = data.nEvil[0];
    data_buffer[17] = data.nEvil[1];
    data_buffer[18] = data.nScore[0];
    data_buffer[19] = data.nScore[1];
    data_buffer[30] = 0x0D;
    data_buffer[31] = 0x0A;*/
}

void Logic::run(const LocateResult *info)
{
    constexpr double OVERSPEED_THRESHOLD = 40;
    constexpr double FORBIDDEN_AREA_RADIUS = 50;
    constexpr double PUNISHMENT_PER_ROUND = 4;
    constexpr int EVIL_THRESHOLD = 100;

    // The frame is downsampled
    // multiply each component by 2 to get the original position
    auto mult2=[](QPointF p)->QPointF{return {2*p.x(), 2*p.y()};};

    if (m_status != Running)
        return;

    if (m_elapsedTime >= Config::Get().getTimeLimit())
    {
        stop(true);
        return;
    }

    m_elapsedTime += 1;
    emit elapsedTimeChanged(m_elapsedTime);

    if (info == nullptr)
    {
        reduceRestStop(0);
        reduceRestStop(1);
        return;
    }

    m_ball = info->ball;
    m_car[0] = info->cars[0];
    m_car[1] = info->cars[1];

    m_ball.raw_center = mult2(m_ball.raw_center);
    m_car[0].raw_center = mult2(m_car[0].raw_center);
    m_car[1].raw_center = mult2(m_car[1].raw_center);

    emit rawBallPosChanged(m_ball.raw_center);
    emit rawCarAPosChanged(m_car[0].raw_center);
    emit rawCarBPosChanged(m_car[1].raw_center);

    int defend_side = 1-m_shootSide;

    auto& rpt_attack = info->cars[m_shootSide];
    auto& rpt_defend = info->cars[defend_side];

    if (!rpt_defend.located || !rpt_defend.located)
    {
        reduceRestStop(0);
        reduceRestStop(1);
        return;
    }

    double dis_attack = hypot(rpt_attack.center.y()-105, rpt_attack.center.x());
    double dis_defend = hypot(rpt_defend.center.y()-105, rpt_defend.center.x());


    bool punish_overspeed =
            rpt_defend.speed_stable &&
            rpt_defend.speed > OVERSPEED_THRESHOLD;

    bool punish_forbidden_area =
            m_elapsedTime > m_shoot_side_protected &&
            dis_defend < FORBIDDEN_AREA_RADIUS &&
            dis_attack > FORBIDDEN_AREA_RADIUS;

    double new_evil_defend = m_evil[defend_side];
    new_evil_defend += punish_overspeed || punish_forbidden_area ?
                PUNISHMENT_PER_ROUND :
                -0.8;

    setEvil(defend_side, std::max(new_evil_defend, 0.0));

    if (m_evil[defend_side]>EVIL_THRESHOLD)
    {
        setEvil(defend_side, 0);
        m_stopCount[defend_side] += 1;

        m_shoot_side_protected = m_elapsedTime + 30;
        int new_restStop = m_restStop[defend_side];
        new_restStop += 10*min(2*m_stopCount[defend_side]*3, 10);
        setRestStop(defend_side, new_restStop);
        return;
    }

    reduceRestStop(defend_side);
}
