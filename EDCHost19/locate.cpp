#include "locate.h"

void ObjectTracker::update(QPointF center, QPointF raw, int64_t timestamp_ms)
{
    m_located = true;
    m_current_center = center;
    m_current_raw = raw;

    if (last_timestamp_ms+2000 < timestamp_ms)
    {
        head = 0;
        queue_full = false;
    }

    last_timestamp_ms = timestamp_ms;

    if (!queue_full)
    {
        if (head < CAPICITY-1)
        {
            auto& cur = queue[head++];
            cur.center = center;
            cur.timestamp_ms = timestamp_ms;
            return;
        }

        queue_full = true;
    }

    //circular queue must be full

    auto& cur = queue[head++];
    cur.center = center;
    cur.timestamp_ms = timestamp_ms;

    if (head==CAPICITY)
        head = 0;
}

void ObjectTracker::update_failure(int64_t timestamp_ms)
{
    Q_UNUSED(timestamp_ms);
    m_located = false;
}

void ObjectTracker::genReport(Report* report) const
{
    report->located = m_located;
    report->raw_center = m_current_raw;
    report->center = m_current_center;
    report->speed = 0;

    if ( !m_located || !queue_full)
    {
        report->speed_stable = false;
        return;
    }
    auto& prev = queue[head];
    auto& cur  = queue[head==0 ? CAPICITY-1 : head-1];
    double time_span_s = (cur.timestamp_ms - prev.timestamp_ms)/1000.0;
    report->speed_stable = true;
    report->speed = hypot(cur.center.x()-prev.center.x(),
                        cur.center.y()-prev.center.y())/time_span_s;
}

QString& operator+=(QString &str, const ObjectTracker::Report &obj)
{
    str += QStringLiteral("located: %1\n").arg(obj.located);
    str += QStringLiteral("vel stable: %1\n").arg(obj.speed_stable);
    str += QStringLiteral("center: %1, %2\n").arg(obj.center.x()).arg(obj.center.y());
    str += QStringLiteral("cam   : %1, %2\n").arg(obj.raw_center.x()).arg(obj.raw_center.y());
    str += QStringLiteral("speed : %1\n").arg(obj.speed);

    return str;
}
