#include "locate.h"



void ObjectTracker::update(QPointF center, QPointF raw, const QTime &timestamp)
{
    m_located = true;
    m_current_center = center;
    m_current_raw = raw;
}

void ObjectTracker::update_failure(QTime const& timestamp)
{
    m_located = false;
}

void ObjectTracker::genReport(Report* report)
{
    report->raw_center = m_current_raw;
    report->center = m_current_center;
    report->velocity = {};
    report->located = m_located;
    report->vel_stable = false;
}
