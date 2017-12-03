#include "camera.h"
#include "globalconfig.h"
#include "controller.h"
#include <QtCore>
#include <QtDebug>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>


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

    //qWarning() << surfaceFormat.scanLineDirection();

    ImgprocThread* worker = qobject_cast<ImgprocThread*>(this->worker);

    if (!input->map(QAbstractVideoBuffer::ReadOnly))
    {
        qCritical() << "Unable to map VideoFrame";
        goto ret;
    }


    {
        MutexTryLocker _(worker->cv_mutex);
        if (!_.isLocked())
            goto unmap;

        worker->frame_timestamp_ms = GetElapsedTime(); //TODO

        switch (surfaceFormat.pixelFormat())
        {
        case QVideoFrame::Format_YUV420P:
        {
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
            break;
        }

        case QVideoFrame::Format_RGB32:
        {
            cv::Mat_<cv::Vec4b> F(
                        input->height(),
                        input->width(),
                        reinterpret_cast<cv::Vec4b*>(input->bits()),
                        input->bytesPerLine());
            cv::Mat_<cv::Vec4b> half, half_fliped;
            cv::resize(F, half, cv::Size(input->width()/2, input->height()/2));
            cv::flip(half, half_fliped, 0); // flip vertically
            cv::cvtColor(half_fliped, worker->frame, CV_BGR2YUV);
            break;
        }

        default:
            qCritical() << surfaceFormat.pixelFormat() << ",expected YUV420p";
            goto unmap;
        }
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

    GetProcConfigMutate()->reset_debug();
    proc.cvt = &cvt;

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
            qDebug() << "updated";
        }
        else if (succeeded)
        {
            cvt.updateParam();
            qDebug() << "updated";
        }

        LocateResult* r = proc.Locate(frame, frame_timestamp_ms);

        emit ResultEmitted(r);

        continue;
    }
}
