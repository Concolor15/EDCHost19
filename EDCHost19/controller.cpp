#include "stdafx.h"
#include "controller.h"

#include <opencv2/highgui.hpp>
#include <CoordinateConverter.h>
#include "globalconfig.h"
#include "camera.h"
#include "logic.h"

Controller* Controller::inst = 0;

Controller::Controller(QObject *parent) : QObject(parent)
{
    logic = new Logic(this);
}

void Controller::init()
{
    elapsedTimer.start();

    imgThread = new ImgprocThread();
    imgThread->start();

    QObject::connect(
                imgThread, &ImgprocThread::ResultEmitted,
                this, &Controller::imgproc_handle);

    initSerial();

    QObject::connect(&timer, &QTimer::timeout, this, &Controller::serialport_timer_handle);
    timer.start(100);

    initComponent();
}

void Controller::imgproc_handle(LocateResult* _data)
{
    LocateResult* r = _data;

    QString debugInfo;
    debugInfo += QStringLiteral("Ball:\n");
    debugInfo += r->ball;

    debugInfo += QStringLiteral("\nCarA:\n");
    debugInfo += r->cars[0];

    debugInfo += QStringLiteral("\nCarB:\n");
    debugInfo += r->cars[1];

    emit CameraDebugInfoEmitted(debugInfo);

    if (lastResult==nullptr)
    {
        delete lastResult;
        lastResult = nullptr;
    }

    lastResult = r;
}

static void appendSerialDebugInfo(QString& str, uint8_t (&data)[32])
{
    static char hex[16+1]="0123456789ABCDEF";

    auto f = [&](QString prefix, int start, int end) {
        str += prefix;
        for (int i=start; i<end; i++)
            str.append(hex[data[i]>>4])
                    .append(hex[data[i] & 0xF])
                    .append(" ");
        str += "\n";
    };

    f("       : ", 0, 3);
    f("carA   : ", 3, 6);
    f("carB   : ", 6, 9);
    f("ball   : ", 9 ,12);
    f("stopA  : ", 12, 14);
    f("stopB  : ", 14, 16);
    f("evilAB : ", 16, 18);
    f("scoreAB  ", 18, 20);
    f("  ", 20, 26);
    f("  ", 26, 32);
}

void Controller::serialport_timer_handle()
{
    logic->run(lastResult);

    if (lastResult==nullptr)
    {
        delete lastResult;
        lastResult = nullptr;
    }

    logic->packToByteArray(data_buffer);
    buffer_has_data = true;

    if (buffer_has_data)
    {
        QString debugInfo;
        debugInfo.reserve(500);

        debugInfo += QTime::currentTime().toString() + "\n";

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

            if (!sp.flush())
            {
                debugInfo += QStringLiteral("flush failed!\n");
                goto send;
            }

            debugInfo += QStringLiteral("succeed\n");
        }

send:
        appendSerialDebugInfo(debugInfo, data_buffer);
        emit SerialDebugInfoEmitted(debugInfo);
    }
    else
    {
        emit SerialDebugInfoEmitted(QStringLiteral("No Data Send"));
    }

}

void Controller::initSerial()
{
    sp.setBaudRate(QSerialPort::Baud115200);
    sp.setDataBits(QSerialPort::Data8);
    sp.setParity(QSerialPort::NoParity);
    sp.setFlowControl(QSerialPort::NoFlowControl);
    sp.setStopBits(QSerialPort::OneStop);

    auto list = QSerialPortInfo::availablePorts();
    if (list.empty())
    {
        qWarning() << "no available serialport";
        return;
    }

    sp.setPortName(list.first().portName());

    if (!sp.open(QIODevice::WriteOnly)) {
        qCritical() << sp.error();
    }
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
    QQmlEngine::setObjectOwnership(inst, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(inst->logic, QQmlEngine::CppOwnership);
    qmlRegisterSingletonType<Controller>(
                "my.uri", 1, 0, "Ctrl",
                [](QQmlEngine*, QJSEngine*)->QObject*{
        return inst;
    });
    qmlRegisterSingletonType<Logic>(
                "my.uri", 1, 0, "Logic",
                [](QQmlEngine*, QJSEngine*)->QObject*{
        return inst->logic;
    });
    qmlRegisterSingletonType<Config>(
                "my.uri", 1, 0, "Config",
                [](QQmlEngine*, QJSEngine*)->QObject*{
        return &Config::Get();
    });
    qmlRegisterType<MyFilter>("my.uri", 1, 0, "Filter");

    engine = new QQmlEngine(this);
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

//void Controller::sendLater()
//{


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
//}

void Controller::setPerspective(
        QPointF p1,
        QPointF p2,
        QPointF p3,
        QPointF p4)
{
    auto* param = new CoordinateConverter::Param;
    param->CameraSize = {0, 0};
    param->LogicSize = {297, 210};
    param->CornersInCamera[0] = p1;
    param->CornersInCamera[1] = p2;
    param->CornersInCamera[2] = p3;
    param->CornersInCamera[3] = p4;

    imgThread->coord_param.set_heap_pointer(param);
}

void Controller::setSerial(bool openOrClose, QString serialName)
{   
    if (openOrClose)
    {
        if (serialName != "")
            sp.setPortName(serialName);
        if (!sp.open(QIODevice::WriteOnly))
            qCritical() << sp.errorString();
    }
    else
    {
        sp.close();
        sp.clearError();
    }
}

void Controller::toggleCamera()
{
    if (cam != nullptr)
    {
        cam->unload();
        cam->deleteLater();
        cam = nullptr;
        return;
    }

    auto devices = QCameraInfo::availableCameras();

    if (devices.empty())
    {
        qCritical() << "no camera available!";
        return;
    }

    cam = new MyCamera(devices.last(), this);
    //cam->load();

    emit cameraChanged(cam);
}

MyCamera *Controller::getCamera()
{
    return cam;
}

void Controller::setCamera(MyCamera* newCamera)
{
    if (cam == newCamera) return;

    delete cam;

    cam = newCamera;
    emit cameraChanged(newCamera);
}

int64_t GetElapsedTime()
{
    auto time = GetController()->elapsedTimer.elapsed();
    //qWarning() << time;
    return time;
}
