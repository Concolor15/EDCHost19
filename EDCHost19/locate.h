#ifndef LOCATE_H
#define LOCATE_H

#include <QtCore>

class ObjectTracker
{
public:
    struct Report
    {
        QPointF raw_center;
        QPointF center;
        QPointF velocity;
        bool located;
        bool vel_stable;
    };

    void update(QPointF center, QPointF raw, QTime const& timestamp);
    void update_failure(QTime const& timestamp);
    void genReport(Report* report);

private:
    bool m_located = false;
    bool queue_full = false;
    QPointF m_current_center = {};
    QPointF m_current_raw = {};

    struct queueItem {
        int64_t timestamp;
        QPointF center;
    };
    constexpr static int CAPICITY = 10;
    queueItem queue[CAPICITY];
    int head, tail = 0;
};

struct LocateResult
{
    ObjectTracker::Report ball;
    ObjectTracker::Report cars[2];

    QTime timestamp;
};

#endif // LOCATE_H
