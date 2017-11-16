#include "globalconfig.h"

Config* Config::inst = nullptr;

static QString readString(QJsonObject& json, QString name)
{
    auto iter = json.find(name);

    assert(iter->type() == QJsonValue::String);

    return iter->toString();
}

static int readInt(QJsonObject& json, QString name)
{
    auto iter = json.find(name);

    assert(iter->type() == QJsonValue::Double);

    return static_cast<int>(iter->toDouble());
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
