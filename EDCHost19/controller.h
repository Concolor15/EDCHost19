#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QtCore>
#include <QtSerialPort>
#include "GlobalType.h"
#include "MainLogic.h"
#include "MyCamera.h"
#include "globalconfig.h"

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MyCamera* camera READ getCamera NOTIFY cameraChanged)
private:
    explicit Controller(QObject *parent = nullptr);
    static Controller* inst;

    MainLogic logic;

    QTimer timer;

    MyCamera* cam;
    ProcConfig cv_param;

    QSerialPort sp;
    QByteArray data_buffer;
    bool buffer_has_data;
    void serialport_timer_handle();

    void setCamera(MyCamera* newCamera);

    void initCv_debug();
public:
    friend Controller* GetController();
    static void Init();
    static void Destroy();

    MyCamera* getCamera() const {return cam;}

    MainLogic& GetLogic() {return logic;}

    void sendLater(MatchInfo const& info);

signals:
    void cameraChanged(MyCamera* newCamera);
    void SerialPortInfoUpdated(QString info);
public slots:
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
