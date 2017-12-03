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
        double speed;
        bool located;
        bool speed_stable;

        friend QString& operator+=(QString& str, Report const& obj);
    };

    void update(QPointF center, QPointF raw, int64_t timestamp_ms);
    void update_failure(int64_t timestamp_ms);
    void genReport(Report* report);

private:
    bool m_located = false;
    bool queue_full = false;
    QPointF m_current_center = {};
    QPointF m_current_raw = {};

    int64_t last_timestamp_ms;

    struct queueItem {
        int64_t timestamp_ms;
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

    int64_t timestamp;
};

#endif // LOCATE_H
