#include "stdafx.h"
#include "config.h"
#include "controller.h"

#include <opencv2/highgui.hpp>
#include <CoordinateConverter.h>

Controller* Controller::inst = 0;

Controller::Controller(QObject *parent) : QObject(parent)
{
    auto devices = QCameraInfo::availableCameras();
    cam = new MyCamera(devices[devices.size()-1], this);

    auto* imgproc = cam->imageProcessing();
    imgproc->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceManual);
    imgproc->setManualWhiteBalance(4000);

    imgThread = new ImgprocThread();
    imgThread->start();
    imgThread->InitCv();
    QObject::connect(
                imgThread, &ImgprocThread::ResultEmitted,
                this, &Controller::imgproc_handle);

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

    initComponent();

    logic.start();
}

void Controller::imgproc_handle(LocateResult* _data, cv::Mat* src1, cv::Mat* src2)
{
    LocateResult* r = _data;
    CameraInfo info;
    info.posBall = r->logic_ball_center.toPoint();
    info.posCar[0] = r->logic_cars_center[0];
    info.posCar[1] = r->logic_cars_center[1];

    logic.run(info);
    sendLater();

    QString debugInfo;
    //debugInfo += QString("posBall: (%1, %2)\n").ar

    delete _data;

    cv::imshow("show", *src1);
    cv::imshow("black", *src2);
}

void Controller::serialport_timer_handle()
{
    if (buffer_has_data)
    {
        QString debugInfo = QTime::currentTime().toString() + "\n";

        buffer_has_data = false;

        {
        if (!sp.isOpen())
        {
            debugInfo += QStringLiteral("device not open!\n");
            goto send;
        }

        if (!sp.isWritable())
        {
            debugInfo += QStringLiteral("device not writable!\n");
            goto send;
        }

        auto count = sp.write((const char*)data_buffer, sizeof(data_buffer));

        if (count == -1)
        {
            debugInfo += QStringLiteral("device write failed!\n");
            goto send;
        }

        if (count != sizeof(data_buffer))
        {
            debugInfo += QStringLiteral("write partly failed: sent %1\n").arg(count);
            goto send;
        }

        debugInfo += QStringLiteral("succeed\n");
        }

    send:

        static char hex[16+1]="0123456789ABCDEF";

        for (int i=0;i<8;i++)
        {
            for (int j=0;j<4;j++)
            {
                uint8_t v = data_buffer[4*i+j];
                debugInfo.append(hex[v >> 4]);
                debugInfo.append(hex[v & 0xF]);
            }
            debugInfo += '\n';
        }
        emit SerialDebugInfoEmitted(debugInfo);
    }
    else
    {
        emit SerialDebugInfoEmitted("No Data Send");
    }

    //logic.Run(CameraInfo());
    //this->sendLater(logic.GetInfo());
}

QQuickWindow* Controller::getMainWindow()
{
    if (mainWindow == nullptr)
        mainWindow = qobject_cast<QQuickWindow*>(mainWindowComponent->create());

    return mainWindow;
}

QQuickWindow* Controller::getMatchWindow()
{
    if (matchWindow == nullptr)
        matchWindow = qobject_cast<QQuickWindow*>(matchWindowComponent->create());

    return matchWindow;
}

QQuickWindow* Controller::getProbeWindow()
{
    if (probeWindow == nullptr)
        probeWindow = qobject_cast<QQuickWindow*>(probeWindowComponent->create());

    return probeWindow;
}

void Controller::initComponent()
{
    static auto* this_inst = this;
    qmlRegisterSingletonType<Controller>("my.uri", 1, 0, "Ctrl", [](QQmlEngine*, QJSEngine*)->QObject*{return this_inst;});
    qmlRegisterType<Logic>("my.uri", 1, 0, "Logic");
    qmlRegisterType<MyFilter>("my.uri", 1, 0, "Filter");

    engine = new QQmlEngine(this);
    engine->rootContext()->setContextProperty("logic", &logic);

    QObject::connect(engine, &QQmlEngine::quit, qGuiApp, &QGuiApplication::quit);

    mainWindowComponent = new QQmlComponent(engine, QUrl("qrc:/Main.qml"), this);
    matchWindowComponent = new QQmlComponent(engine, QUrl("qrc:/MatchUI.qml"), this);
    probeWindowComponent = new QQmlComponent(engine, QUrl("qrc:/Debug.qml"), this);

    if (mainWindowComponent->isError())
        qCritical() << mainWindowComponent->errors();

    if (matchWindowComponent->isError())
        qCritical() << matchWindowComponent->errors();

    if (probeWindowComponent->isError())
        qCritical() << probeWindowComponent->errors();

    //getMainWindow()->show();
    getMatchWindow()->show();
}

void Controller::sendLater()
{
    logic.packToByteArray(data_buffer);
    buffer_has_data = true;

    /*
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
    */
}

void Controller::setPerspective(
        QPointF p1,
        QPointF p2,
        QPointF p3,
        QPointF p4)
{
    qWarning() << p1 << p2 << p3 << p4;
    auto* param = new CoordinateConverter::Param;
    param->CameraSize = {0, 0};
    param->LogicSize = {297, 210};
    param->CornersInCamera[0] = p1;
    param->CornersInCamera[1] = p2;
    param->CornersInCamera[2] = p3;
    param->CornersInCamera[3] = p4;

    imgThread->coord_param.set_heap_pointer(param);
}

void Controller::restartSerial()
{
    sp.close();
    sp.clearError();

    if (!sp.open(QIODevice::WriteOnly))
        qCritical() << sp.errorString();
}
