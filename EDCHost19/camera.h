#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QtCore>
#include <QtMultimedia>
#include <opencv2/core/types.hpp>
#include "type.h"
#include "imgproc.h"
#include "globalconfig.h"

class MyCamera: public QCamera
{
    Q_OBJECT
    Q_PROPERTY(QCamera* mediaObject READ getMediaObject)
public:
    explicit MyCamera(QObject* parent = Q_NULLPTR):
        QCamera(parent) { }
    explicit MyCamera(const QByteArray &deviceName, QObject *parent = Q_NULLPTR):
        QCamera(deviceName, parent) { }
    explicit MyCamera(const QCameraInfo &cameraInfo, QObject *parent = Q_NULLPTR):
        QCamera(cameraInfo, parent) { }
    explicit MyCamera(QCamera::Position position, QObject *parent = Q_NULLPTR):
        QCamera(position, parent) { }

    QCamera* getMediaObject() {return this; }
private:
    //QPointer<QAbstractVideoSurface> m_surface;
};


class MyFilter: public QAbstractVideoFilter
{
    Q_OBJECT
public:
    explicit MyFilter(QObject* parent = Q_NULLPTR):
        QAbstractVideoFilter(parent) { }
    QVideoFilterRunnable* createFilterRunnable() override;
};

class MyFilterRunnable: public QVideoFilterRunnable
{
    QThread* worker;
public:
    explicit MyFilterRunnable(QThread* worker): worker(worker) { }
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) override;
};



class ImgprocThread: public QThread
{
    Q_OBJECT
    friend class MyFilterRunnable;
public:
    AtomicData<ProcConfig> _config;
    AtomicData<CoordinateConverter::Param> coord_param;
    void setDebugEnabled(bool newDebugEnabled);
private:
    static QMutex inst_lock;
    QMutex cv_mutex;
    QWaitCondition cv;

    cv::Mat frame;
    CoordinateConverter cvt;
    ProcConfig config;
    ImgProc proc;

    int64_t frame_timestamp_ms;

    bool debugEnabled = false;

    void run() override;

signals:
    void ResultEmitted(LocateResult* result);
};

#endif // MYCAMERA_H
