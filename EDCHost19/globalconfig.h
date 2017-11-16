#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QtCore>
struct ProcConfig
{
    int red_reverse;

    int ball_hue_lb;
    int ball_hue_ub;

    int car1_hue_lb;
    int car1_hue_ub;

    int car2_hue_lb;
    int car2_hue_ub;

    int car1_s_lb;
    int car2_s_lb;
    int ball_s_lb;

    int car1_v_lb;
    int car2_v_lb;
    int ball_v_lb;

    int area_car_lb;
    int area_ball_lb;

    void reset_debug()
    {
        red_reverse = 0;
        ball_hue_lb = 0;
        ball_hue_ub = 0;
        car1_hue_lb = 0;
        car1_hue_ub = 0;
        car2_hue_lb = 0;
        car2_hue_ub = 0;
        car1_s_lb = 0;
        car2_s_lb = 0;
        ball_s_lb = 0;
        ball_v_lb = 0;
        car1_v_lb = 0;
        car2_v_lb = 0;
        area_car_lb = 100;
        area_ball_lb = 100;
    }
};

class Config
{
    static Config* inst;
public:
    static Config const& Get();
    static Config* GetMutatable();
    void LoadFromFile(QString filename);
    void SaveToFile(QString filename) const;

    QString video_dev_name;
    QString serial_dev_name;
};

inline Config const& Config::Get()
{
    return *inst;
}

inline Config* Config::GetMutatable()
{
    return inst;
}

#endif // GLOBALCONFIG_H
