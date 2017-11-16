#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QtCore>
#include <QtMultimedia>

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
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) override;
};

inline QVideoFilterRunnable* MyFilter::createFilterRunnable()
{
    return new MyFilterRunnable();
}

class ImgprocThread: QThread
{
    Q_OBJECT
    QMutex* inst_lock;
    QMutex* cv_lock;
    QWaitCondition* cv;
    QAtomicPointer<void>* param;
    void run() override;
};

#endif // MYCAMERA_H
