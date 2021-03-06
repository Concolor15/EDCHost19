#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtCore>
#include <QtQml>
#include <QtQuick>
#include <QtSerialPort>
#include "GlobalType.h"
#include "logic.h"
#include "MyCamera.h"
#include "globalconfig.h"

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
    static void Init();
    static void Destroy();

    ImgprocThread* imgThread;

    Logic& GetLogic() {return logic;}

    void sendLater();

    Q_INVOKABLE void setPerspective(
                QPointF p1,
                QPointF p2,
                QPointF p3,
                QPointF p4);

    Q_INVOKABLE void restartSerial();

    MyCamera* getCamera() const {return cam;}
    QQuickWindow* getMainWindow();
    QQuickWindow* getMatchWindow();
    QQuickWindow* getProbeWindow();

signals:
    void cameraChanged(MyCamera* newCamera);
    void CameraDebugInfoEmitted(QString info);
    void SerialDebugInfoEmitted(QString info);

public slots:
    void imgproc_handle(LocateResult* data, cv::Mat* src1, cv::Mat* src2);
    void serialport_timer_handle();


private:
    QQmlEngine* engine;
    QQmlComponent* mainWindowComponent;
    QQmlComponent* matchWindowComponent;
    QQmlComponent* probeWindowComponent;
    QPointer<QQuickWindow> mainWindow;
    QPointer<QQuickWindow> matchWindow;
    QPointer<QQuickWindow> probeWindow;

    Logic logic;

    QTimer timer;

    MyCamera* cam;
    ProcConfig cv_param;

    QSerialPort sp;
    uint8_t data_buffer[32];
    bool buffer_has_data {false};

    void setCamera(MyCamera* newCamera);

    void initCv_debug();
    void initComponent();
};

inline Controller* GetController()
{
    return Controller::inst;
}

inline void Controller::Init()
{
    inst = new Controller();
}

inline void Controller::Destroy()
{
    delete inst;
    inst = nullptr;
}

inline void Controller::setCamera(MyCamera* newCamera)
{
    if (cam == newCamera) return;

    cam = newCamera;
    emit cameraChanged(newCamera);
}

#endif // CONTROLLER_H
