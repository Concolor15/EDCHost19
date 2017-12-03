#include "globalconfig.h"

#include <cstddef>

Config* Config::inst = nullptr;

static QString readString(QJsonObject& json, QString name)
{
    auto iter = json.find(name);

    Q_ASSERT(iter->type() == QJsonValue::String);

    return iter->toString();
}

static int readInt(QJsonObject& json, QString name)
{
    auto iter = json.find(name);

    Q_ASSERT(iter->type() == QJsonValue::Double);

    return static_cast<int>(iter->toDouble());
}

Config::Config(QObject *parent): QObject(parent)
{

}

void Config::LoadFromFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();

    QJsonObject doc = QJsonDocument::fromJson(data).object();

    video_dev_name = readString(doc, "video");
    serial_dev_name = readString(doc, "serial");
}

void Config::setProcConfigByString(QString str)
{
    auto valid = [](QChar c){return ('0'<=c && c<='9') || ('A'<=c && c<='F');};
    auto hex2num = [](QChar c){
        return '0' <= c && c<='9' ? c.unicode()-L'0' : c.unicode()-L'A'+10;
    };

    size_t size = sizeof(ProcConfig);
    if (str.size()!=size*2)
    {
        return;
    }


    if (!std::all_of(str.begin(), str.end(), valid))
    {
        return;
    }

    uint8_t* data = reinterpret_cast<uint8_t*>(&procConfig);
    for (int i=0;i<size;i++)
        data[i] = hex2num(str[i*2])*16+hex2num(str[i*2+1]);
}

QString Config::getProcConfigByString()
{
    size_t size = sizeof(ProcConfig);
    QString str;
    str.reserve(size*2);
    uint8_t const* data = reinterpret_cast<uint8_t*>(&procConfig);

    const char* hex = "0123456789ABCDEF";

    for (int i=0;i<size;i++)
        str.append(hex[data[i]>>4]).append(hex[data[i] & 0xF]);

    return str;
}

void Config::setCvDebugEnabled(bool newDebugEnabled)
{
    using namespace cv;

    bool oldDebugEnabled = cvDebugEnabled.fetchAndStoreOrdered(newDebugEnabled) != 0;

    if (newDebugEnabled == oldDebugEnabled)
        return;

    auto& config = this->procConfig;

    using namespace cv;
    if (newDebugEnabled)
    {
        namedWindow("show");
        namedWindow("black");
        namedWindow("control");
        namedWindow("control2");
        cv::createTrackbar("ball_Y_lb", "control", &config.ball_Y_lb, 255);
        cv::createTrackbar("ball_Y_ub", "control", &config.ball_Y_ub, 255);
        cv::createTrackbar("ball_U_lb", "control", &config.ball_U_lb, 255);
        cv::createTrackbar("ball_U_ub", "control", &config.ball_U_ub, 255);
        cv::createTrackbar("ball_V_lb", "control", &config.ball_V_lb, 255);
        cv::createTrackbar("ball_V_ub", "control", &config.ball_V_ub, 255);

        createTrackbar("area_car_lb", "control", &config.area_car_lb, 1000);
        createTrackbar("area_ball_lb", "control", &config.area_ball_lb, 100);
        createTrackbar("area_ball_ub", "control", &config.area_ball_ub, 300);

        createTrackbar("area_ball_r_lb", "control", &config.area_ball_r_lb, 100);

        cv::createTrackbar("car1_Y_lb", "control2", &config.car1_Y_lb, 255);
        cv::createTrackbar("car1_Y_ub", "control2", &config.car1_Y_ub, 255);
        cv::createTrackbar("car1_U_lb", "control2", &config.car1_U_lb, 255);
        cv::createTrackbar("car1_U_ub", "control2", &config.car1_U_ub, 255);
        cv::createTrackbar("car1_V_lb", "control2", &config.car1_V_lb, 255);
        cv::createTrackbar("car1_V_ub", "control2", &config.car1_V_ub, 255);

        cv::createTrackbar("car2_Y_lb", "control2", &config.car2_Y_lb, 255);
        cv::createTrackbar("car2_Y_ub", "control2", &config.car2_Y_ub, 255);
        cv::createTrackbar("car2_U_lb", "control2", &config.car2_U_lb, 255);
        cv::createTrackbar("car2_U_ub", "control2", &config.car2_U_ub, 255);
        cv::createTrackbar("car2_V_lb", "control2", &config.car2_V_lb, 255);
        cv::createTrackbar("car2_V_ub", "control2", &config.car2_V_ub, 255);
    }
    else
    {
        destroyAllWindows();
    }

}
