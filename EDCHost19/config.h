#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <QSize>

constexpr int GAME_TIME = 36000;

constexpr const char* SERIAL_PORT_NAME = "ttyUSB0";
constexpr QSize CAMERA_SIZE(1920, 1080);
constexpr QSize CAMERA_DOWNSAMPLE_SIZE(960, 540);

constexpr int nFPS = 10;

constexpr int nViewWidth = 800;
constexpr int nViewHeight = 600;
constexpr int nLogicWidth = 297;
constexpr int nLogicHeight = 210;

#endif // CONFIG_H
