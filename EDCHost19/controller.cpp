#include "stdafx.h"
#include "config.h"
#include "controller.h"

#include <opencv2/highgui.hpp>

Controller* Controller::inst = 0;

Controller::Controller(QObject *parent) : QObject(parent),
    timer(this),
    cam(nullptr),
    sp(this),
    data_buffer(32, 0),
    buffer_has_data(false)
{
    auto devices = QCameraInfo::availableCameras();
    cam = new MyCamera(devices[devices.size()-1], this);

    auto* imgproc = cam->imageProcessing();
    imgproc->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceManual);
    imgproc->setManualWhiteBalance(4000);


    sp.setPortName(SERIAL_PORT_NAME);
    sp.setBaudRate(QSerialPort::Baud115200);
    sp.setDataBits(QSerialPort::Data8);
    sp.setParity(QSerialPort::NoParity);
    sp.setFlowControl(QSerialPort::NoFlowControl);
    sp.setStopBits(QSerialPort::OneStop);

    if (!sp.open(QIODevice::ReadWrite)) {
        qInfo() << sp.error();
    }

    QObject::connect(&timer, &QTimer::timeout, this, &Controller::serialport_timer_handle);
    timer.start(100);
}

void Controller::serialport_timer_handle()
{
    if (buffer_has_data)
    {
        buffer_has_data = false;

        sp.write(data_buffer);

        QString debugInfo = QTime::currentTime().toString() + "\n" + QString(data_buffer.toHex());
        emit SerialPortInfoUpdated(debugInfo);
    }
    else
    {
        emit SerialPortInfoUpdated("No Data Send");
    }
}

void Controller::initCv_debug()
{
    cv_param.reset_debug();
    cv::namedWindow("show");
    cv::namedWindow("black");
    cv::namedWindow("control", CV_WINDOW_NORMAL);
    cv::createTrackbar("car1_red_reverse", "control", &cv_param.red_reverse, 1);
    cv::createTrackbar("ball_hue_lb", "control", &cv_param.ball_hue_lb, 180);
    cv::createTrackbar("ball_hue_ub", "control", &cv_param.ball_hue_ub, 180);
    cv::createTrackbar("car1_hue_lb", "control", &cv_param.car1_hue_lb, 180);
    cv::createTrackbar("car1_hue_ub", "control", &cv_param.car1_hue_ub, 180);
    cv::createTrackbar("car2_hue_lb", "control", &cv_param.car2_hue_lb, 180);
    cv::createTrackbar("car2_hue_ub", "control", &cv_param.car2_hue_ub, 180);
    cv::createTrackbar("ball_s_lb", "control", &cv_param.ball_s_lb, 255);
    cv::createTrackbar("car1_s_lb", "control", &cv_param.car1_s_lb, 255);
    cv::createTrackbar("car2_s_lb", "control", &cv_param.car2_s_lb, 255);
    cv::createTrackbar("ball_v_lb", "control", &cv_param.ball_v_lb, 255);
    cv::createTrackbar("car1_v_lb", "control", &cv_param.car1_v_lb, 255);
    cv::createTrackbar("car2_v_lb", "control", &cv_param.car2_v_lb, 255);
    cv::createTrackbar("area_car_lb", "control", &cv_param.area_car_lb, 1000);
    cv::createTrackbar("area_ball_lb", "control", &cv_param.area_ball_lb, 200);
}

void Controller::sendLater(MatchInfo const& data)
{
    buffer_has_data = true;

    auto const& pos = data.posObjs;
    data_buffer[0] = 0xFC | (data.binShootout << 1) | (data.shootSide^1);
    data_buffer[1] = (data.quaGameStatus << 6) | (data.nTimeByRounds >> 8);
    data_buffer[2] = data.nTimeByRounds & 0xFF;

    data_buffer[3] = pos.posCar1.x();
    data_buffer[4] = pos.posCar1.y() >> 8;
    data_buffer[5] = pos.posCar1.y() & 0xFF;

    data_buffer[6] = pos.posCar2.x();
    data_buffer[7] = pos.posCar2.y() >> 8;
    data_buffer[8] = pos.posCar2.y() & 0xFF;

    data_buffer[9] = pos.posBall.x();
    data_buffer[10] = pos.posBall.y() >> 8;
    data_buffer[11] = pos.posBall.y() & 0xFF;

    data_buffer[12] = data.nHaltRound[0] >> 8;
    data_buffer[13] = data.nHaltRound[0] & 0xFF;
    data_buffer[14] = data.nHaltRound[1] >> 8;
    data_buffer[15] = data.nHaltRound[1] & 0xFF;
    data_buffer[16] = data.nEvil[0];
    data_buffer[17] = data.nEvil[1];
    data_buffer[18] = data.nScore[0];
    data_buffer[19] = data.nScore[1];
    data_buffer[30] = 0x0D;
    data_buffer[31] = 0x0A;
}
