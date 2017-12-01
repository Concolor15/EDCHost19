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
