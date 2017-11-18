#include "MyCamera.h"
#include "globalconfig.h"
#include "controller.h"
#include <QtCore>
#include <QtDebug>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class MutexTryLocker {
  QMutex &m_;
  bool locked_;
public:
  MutexTryLocker(QMutex &m) : m_(m), locked_(m.tryLock()) {}
  ~MutexTryLocker() { if (locked_) m_.unlock(); }
  bool isLocked() const { return locked_; }
};

QVideoFilterRunnable* MyFilter::createFilterRunnable()
{
    return new MyFilterRunnable(GetController()->imgThread);
}

QVideoFrame MyFilterRunnable::run(
        QVideoFrame *input,
        const QVideoSurfaceFormat &surfaceFormat,
        RunFlags flags)
{
    Q_UNUSED(surfaceFormat)
    Q_UNUSED(flags)

    ImgprocThread* worker = qobject_cast<ImgprocThread*>(this->worker);

    if (surfaceFormat.pixelFormat() != QVideoFrame::Format_YUV420P)
    {
        qCritical() << surfaceFormat.pixelFormat() << ",expected YUV420p";
        goto ret;
    }

    if (!input->map(QAbstractVideoBuffer::ReadOnly))
    {
        qCritical() << "Unable to map VideoFrame";
        goto ret;
    }

    {
        MutexTryLocker _(worker->cv_mutex);
        if (!_.isLocked())
            goto unmap;

        worker->frame_timestamp = QTime::currentTime();

        cv::Mat_<uint8_t> Y(
                    input->height(),
                    input->width(),
                    input->bits(0),
                    input->bytesPerLine(0));

        cv::Mat_<uint8_t> U(
                    input->height()/2,
                    input->width()/2,
                    input->bits(1),
                    input->bytesPerLine(1));

        cv::Mat_<uint8_t> V(
                    input->height()/2,
                    input->width()/2,
                    input->bits(2),
                    input->bytesPerLine(2));

        cv::Mat_<uint8_t> Y_half;
        cv::resize(Y, Y_half, \
                   cv::Size(input->width()/2, input->height()/2));

        cv::merge(std::vector<cv::Mat>{Y_half,U,V}, worker->frame);

    }
    worker->cv.wakeOne();

unmap:
    input->unmap();
ret:
    return *input;
}

QMutex ImgprocThread::inst_lock;

void ImgprocThread::run()
{
    MutexTryLocker lock(inst_lock);

    if (!lock.isLocked())
    {
        qCritical() << "multiple image processing thread are running!";
        return;
    }

    //proc.InitCv();

    for (;;)
    {
        QMutexLocker _(&cv_mutex);
        cv.wait(&cv_mutex);

        auto* p = cvt.GetParamAddress();
        bool succeeded = coord_param.get_and_clear_to(*p);

        if (frame.cols != p->CameraSize.width() && frame.rows != p->CameraSize.height())
        {
            p->CameraSize = {frame.cols, frame.rows};
            cvt.updateParam();
            qWarning() << "updated";
        }
        else if (succeeded)
        {
            cvt.updateParam();
            qWarning() << "updated";
        }

        proc.Locate(frame);
        LocateResult* r = proc.GetResult();

        r->timestamp = frame_timestamp;

        emit ResultEmitted(r);


        //qWarning() << "running";
        continue;
    }
}
