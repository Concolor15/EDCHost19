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

class Config: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int timeLimit MEMBER m_timeLimit NOTIFY onTimeLimitChanged)

    explicit Config(QObject* parent=Q_NULLPTR);
public:
    static Config* inst;
    static Config& Get();
    static Config* GetMutatable();
    static void static_init();
    static void static_destroy();

    int getTimeLimit() const { return m_timeLimit; }

    void LoadFromFile(QString filename);
    void SaveToFile(QString filename) const;

    ProcConfig procConfig;
    QAtomicInt cvDebugEnabled;


    Q_INVOKABLE void setProcConfigByString(QString str);
    Q_INVOKABLE QString getProcConfigByString();
    Q_INVOKABLE void setCvDebugEnabled(bool newDebugEnabled);

    QString video_dev_name;
    QString serial_dev_name;

signals:
    void onTimeLimitChanged(int newTimeLimit);

private:
    int m_timeLimit = 80000;
    int m_flipped = false;
};



inline Config& Config::Get()
{
    return *inst;
}

inline ProcConfig const* GetProcConfig()
{
    return &Config::inst->procConfig;
}

inline ProcConfig* GetProcConfigMutate()
{
    return &Config::inst->procConfig;
}

inline Config* Config::GetMutatable()
{
    return inst;
}

inline void Config::static_init()
{
    inst = new Config();
}
#endif // GLOBALCONFIG_H
