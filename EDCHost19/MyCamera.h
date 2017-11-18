#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QtCore>
#include <QtMultimedia>
#include <opencv2/core.hpp>
#include "HighResCam.h"
#include "globalconfig.h"

class MyCamera: public QCamera
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ getVideoSurface WRITE setVideoSurface)
public:
    MyCamera(QObject* parent = Q_NULLPTR):
        QCamera(parent) { }
    MyCamera(const QByteArray &deviceName, QObject *parent = Q_NULLPTR):
        QCamera(deviceName, parent) { }
    MyCamera(const QCameraInfo &cameraInfo, QObject *parent = Q_NULLPTR):
        QCamera(cameraInfo, parent) { }
    MyCamera(QCamera::Position position, QObject *parent = Q_NULLPTR):
        QCamera(position, parent) { }
public slots:
    void setVideoSurface(QAbstractVideoSurface* surface);
    QAbstractVideoSurface* getVideoSurface();
private:
    QAbstractVideoSurface* m_surface;
};

inline void MyCamera::setVideoSurface(QAbstractVideoSurface* surface)
{
    m_surface = surface;
    setViewfinder(m_surface);
}

inline QAbstractVideoSurface* MyCamera::getVideoSurface()
{
    return m_surface;
}

class MyFilter: public QAbstractVideoFilter
{
    Q_OBJECT
public:
    MyFilter(QObject* parent = Q_NULLPTR):
        QAbstractVideoFilter(parent) { }
    QVideoFilterRunnable* createFilterRunnable() override;
};

class MyFilterRunnable: public QVideoFilterRunnable
{
    QThread* worker;
public:
    MyFilterRunnable(QThread* worker): worker(worker) { }
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) override;
};

template<typename T>
class AtomicData
{
    //using T=int;
    QAtomicPointer<T> pointer;
public:
    void set_heap_pointer(T* newData)
    {
        T* old = pointer.fetchAndStoreOrdered(newData);
        delete old;
    }

    void set(T const& newData)
    {
        set_heap_pointer(new T(newData));
    }

    T* get_and_clear()
    {
        return pointer.fetchAndStoreOrdered(nullptr);
    }

    bool get_and_clear_to(T& store)
    {
        T* newData=get_and_clear();

        if (newData!=nullptr)
        {
            store = *newData;
            delete newData;
            return true;
        }

        return false;
    }
};

class ImgprocThread: public QThread
{
    Q_OBJECT
    friend class MyFilterRunnable;
public:
    AtomicData<ProcConfig> config;
    AtomicData<CoordinateConverter::Param> coord_param;
private:
    static QMutex inst_lock;
    QMutex cv_mutex;
    QWaitCondition cv;

    cv::Mat frame;
    CoordinateConverter cvt;
    ImgProc proc;

    void run() override;

signals:
    void ResultEmitted(QSharedPointer<LocateResult> result);
};

#endif // MYCAMERA_H
