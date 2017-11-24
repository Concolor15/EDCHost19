#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QtCore>
struct ProcConfig
{
    int ball_Y_lb;
    int ball_Y_ub;
    int ball_U_lb;
    int ball_U_ub;
    int ball_V_lb;
    int ball_V_ub;

    int car1_Y_lb;
    int car1_Y_ub;
    int car1_U_lb;
    int car1_U_ub;
    int car1_V_lb;
    int car1_V_ub;

    int car2_Y_lb;
    int car2_Y_ub;
    int car2_U_lb;
    int car2_U_ub;
    int car2_V_lb;
    int car2_V_ub;

    int area_car_lb;
    int area_ball_lb;
    int area_ball_ub;
    int area_ball_r_lb;

    void reset_debug()
    {
        *this = {};
        area_car_lb = 500;
        area_ball_lb = 30;
        area_ball_ub = 200;
        area_ball_r_lb = 40;
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
