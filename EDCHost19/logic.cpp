#include "logic.h"


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
    emit shouldStopAChanged(getShouldStopA());
    emit shouldStopBChanged(getShouldStopB());
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
    m_stopUntil[0] = 0;
    m_stopUntil[1] = 0;
    m_evil[0] = 0;
    m_evil[1] = 0;
    m_shootSide = 0;
    m_ball = {};
    m_car[0] = {};
    m_car[1] = {};

    updateAll();
}

void Logic::addElapsedTime(int delta)
{
    if (delta == 0)
        return;

    int old = m_elapsedTime;
    m_elapsedTime += delta;

    emit elapsedTimeChanged(m_elapsedTime);

    if (old < m_stopUntil[0] && m_stopUntil[0] <= old+delta)
        emit shouldStopAChanged(false);

    if (old < m_stopUntil[1] && m_stopUntil[1] <= old+delta)
        emit shouldStopBChanged(false);

    if (m_elapsedTime <= m_stopUntil[0])
        emit restStopAChanged(m_stopUntil[0]-m_elapsedTime);

    if (m_elapsedTime <= m_stopUntil[1])
        emit restStopBChanged(m_stopUntil[1]-m_elapsedTime);
}

void Logic::updateStopInfo(int side, int newStopUntil)
{
    Q_ASSERT(side==0 || side==1);
    Q_ASSERT(newStopUntil > std::max(m_elapsedTime, m_stopUntil[side]));

    m_stopUntil[side] = newStopUntil;

    if (side==0)
    {
        emit shouldStopAChanged(true);
        emit restStopAChanged(getRestStopA());
    }
    else
    {
        emit shouldStopBChanged(true);
        emit restStopBChanged(getRestStopB());
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
    constexpr int EVIL_THRESHOLD = 100;

    // The frame is downsampled
    // multiply each component by 2 to get the original position
    auto mult2=[](QPointF p)->QPointF{return {2*p.x(), 2*p.y()};};

    if (m_status != Running)
        return;

    addElapsedTime(1);

    if (info == nullptr)
        return;

    m_ball = info->ball;
    m_car[0] = info->cars[0];
    m_car[1] = info->cars[1];

    m_ball.raw_center = mult2(m_ball.raw_center);
    m_car[0].raw_center = mult2(m_car[0].raw_center);
    m_car[1].raw_center = mult2(m_car[1].raw_center);

    emit rawBallPosChanged(m_ball.raw_center);
    emit rawCarAPosChanged(m_ball.raw_center);
    emit rawCarBPosChanged(m_ball.raw_center);

    int defend_side = 1-m_shootSide;
    int& evil_defend = m_evil[defend_side];

    // TODO: update evil

    if (defend_side==0)
        emit evilAChanged(m_evil[0]);
    else
        emit evilBChanged(m_evil[1]);

    if (evil_defend >= EVIL_THRESHOLD)
    {
        updateStopInfo(defend_side, 0);
    }
}
