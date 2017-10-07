#pragma once

#include <QtWidgets>
#include <QtCore>
#include <QtMultimedia>
#include <vector>
#include <chrono>
#include <opencv2\opencv.hpp>

#include "GlobalType.hpp"
#include "Serial.h"
#include "Camera.h"
#include "MainLogic.h"

#pragma execution_character_set("utf-8")

//#define CAMERA_DEBUG

constexpr int nFPS = 25;