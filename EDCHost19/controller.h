#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtCore>
#include <QtQml>
#include <QtQuick>
#include <QtSerialPort>
#include "util.h"
#include "camera.h"

class MyCamera;
class ImgprocThread;
struct LocateResult;
class Logic;

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MyCamera* camera READ getCamera NOTIFY cameraChanged)
    Q_PROPERTY(QQuickWindow* mainWindow  READ getMainWindow)
    Q_PROPERTY(QQuickWindow* matchWindow READ getMatchWindow)
    Q_PROPERTY(QQuickWindow* probeWindow READ getProbeWindow)

    explicit Controller(QObject *parent = nullptr);
    static Controller* inst;


public:
    friend Controller* GetController();
    static void static_Init();
    static void static_Destroy();

    friend int64_t GetElapsedTime();

    void init();

    ImgprocThread* imgThread;

    Logic* GetLogic() {return logic;}

    Q_INVOKABLE void setPerspective(
                QPointF p1,
                QPointF p2,
                QPointF p3,
                QPointF p4);

    Q_INVOKABLE void setSerial(bool openOrClose, QString serialName);
    Q_INVOKABLE void toggleCamera();

    MyCamera* getCamera();
    QQuickWindow* getMainWindow();
    QQuickWindow* getMatchWindow();
    QQuickWindow* getProbeWindow();

signals:
    void cameraChanged(MyCamera* newCamera);
    void CameraDebugInfoEmitted(QString info);
    void SerialDebugInfoEmitted(QString info);

private slots:
    void imgproc_handle(LocateResult* data);
    void serialport_timer_handle();
    //void cameraErrorHandler();

private:
    QQmlEngine* engine;
    QQmlComponent* mainWindowComponent;
    QQmlComponent* matchWindowComponent;
    QQmlComponent* probeWindowComponent;
    QPointer<QQuickWindow> mainWindow;
    QPointer<QQuickWindow> matchWindow;
    QPointer<QQuickWindow> probeWindow;

    Logic* logic;

    QTimer timer;
    QElapsedTimer elapsedTimer;

    LocateResult* lastResult = nullptr;

    MyCamera* cam = nullptr;

    QSerialPort sp;
    uint8_t data_buffer[32];
    bool buffer_has_data {false};

    void setCamera(MyCamera* newCamera);

    void initSerial();
    void initCv_debug();
    void initComponent();
};

inline Controller* GetController()
{
    return Controller::inst;
}

inline void Controller::static_Init()
{
    inst = new Controller();
    inst->init();
}

inline void Controller::static_Destroy()
{
    delete inst;
    inst = nullptr;
}

int64_t GetElapsedTime();

#endif // CONTROLLER_H
