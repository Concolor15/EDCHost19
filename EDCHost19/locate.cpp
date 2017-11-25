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
        head = 0;
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

void ObjectTracker::genReport(Report* report)
{
    report->raw_center = m_current_raw;
    report->center = m_current_center;
    report->velocity = {};

    if ( !m_located || !queue_full)
    {
        report->vel_stable = false;
        return;
    }
    auto& prev = queue[head];
    auto& cur  = queue[head==0 ? CAPICITY-1 : head-1];
    double time_span_ms = cur.timestamp_ms - prev.timestamp_ms;
    report->vel_stable = true;
    report->velocity = {(cur.center.x()-prev.center.y())/time_span_ms,
                        (cur.center.y()-prev.center.y())/time_span_ms};
}

QString& operator+=(QString &str, const ObjectTracker::Report &obj)
{
    auto hyper = [](QPointF p){return sqrt(p.x()*p.x()+p.y()*p.y());};

    str += QStringLiteral("located: %1\n").arg(obj.located);
    str += QStringLiteral("vel stable: %1\n").arg(obj.vel_stable);
    str += QStringLiteral("center: %1, %2\n").arg(obj.center.x()).arg(obj.center.y());
    str += QStringLiteral("cam   : %1, %2\n").arg(obj.raw_center.x()).arg(obj.raw_center.y());
    str += QStringLiteral("velocity: %1\n").arg(hyper(obj.velocity));

    return str;
}
