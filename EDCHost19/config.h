#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <QSize>

constexpr int GAME_TIME = 36000;

constexpr const char* SERIAL_PORT_NAME = "ttyUSB1";
constexpr QSize CAMERA_SIZE(1920, 1080);
constexpr QSize CAMERA_DOWNSAMPLE_SIZE(960, 540);

#endif // CONFIG_H
