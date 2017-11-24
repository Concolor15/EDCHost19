#include "logic.h"


Logic::Logic(QObject* parent) : QObject(parent)
{
    reset();
}

void Logic::start()
{
    Q_ASSERT(m_status == NotStart || m_status == Finished);

    reset();
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

void Logic::updateAll()
{
    emit statusChanged(m_status);
    emit elapsedTimeChanged(m_elapsedTime);
}

void Logic::reset()
{
    m_status = NotStart;
    m_elapsedTime = 0;
}

void Logic::packToByteArray(uint8_t (&data)[32])
{
    for (auto& x : data) x=0;

    auto writePoint = [&data](int idx, QPointF const& p)
    {
        int x = (int)p.x();
        data[idx] = (uint8_t)p.y();
        data[idx+1] = x >> 8;
        data[idx+2] = x & 0xFF;
    };

    data[0] = 0xFC;
    data[1] = m_elapsedTime >> 8;
    data[2] = m_elapsedTime & 0xFF;

    writePoint(3, m_cars[0]);
    writePoint(6, m_cars[1]);
    writePoint(9, m_ball);

    uint16_t checksum = qChecksum((const char*)data, 28);
    data[28] = checksum >> 8;
    data[29] = checksum & 0xFF;

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
    if (m_status != Running)
        return;

    m_elapsedTime += 1;

    emit elapsedTimeChanged(m_elapsedTime);

    if (info==nullptr)
        return;

    if (info->ball_succeeded)
    {
        m_ball = info->logic_ball_center;
    }

    if (info->cars_succeeded[0])
    {
        m_cars[0] = info->logic_cars_center[0];
    }

    if (info->cars_succeeded[1])
    {
        m_cars[1] = info->logic_cars_center[1];
    }


    emit ballPosChanged(m_ball);
    emit carAPosChanged(m_cars[0]);
    emit carBPosChanged(m_cars[1]);
}
