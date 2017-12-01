#include "camera.h"
#include "globalconfig.h"
#include "controller.h"
#include <QtCore>
#include <QtDebug>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


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

        worker->frame_timestamp_ms = GetElapsedTime(); //TODO

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

void ImgprocThread::setDebugEnabled(bool newDebugEnabled)
{
    using namespace cv;

    if (newDebugEnabled == debugEnabled)
        return;

    debugEnabled = newDebugEnabled;
    proc.debugEnabled = newDebugEnabled;

    auto& config = *GetProcConfigMutate();

    using namespace cv;
    if (newDebugEnabled)
    {
        namedWindow("show");
        namedWindow("black");
        namedWindow("control");
        namedWindow("control2");
        cv::createTrackbar("ball_Y_lb", "control", &config.ball_Y_lb, 255);
        cv::createTrackbar("ball_Y_ub", "control", &config.ball_Y_ub, 255);
        cv::createTrackbar("ball_U_lb", "control", &config.ball_U_lb, 255);
        cv::createTrackbar("ball_U_ub", "control", &config.ball_U_ub, 255);
        cv::createTrackbar("ball_V_lb", "control", &config.ball_V_lb, 255);
        cv::createTrackbar("ball_V_ub", "control", &config.ball_V_ub, 255);

        createTrackbar("area_car_lb", "control", &config.area_car_lb, 1000);
        createTrackbar("area_ball_lb", "control", &config.area_ball_lb, 100);
        createTrackbar("area_ball_ub", "control", &config.area_ball_ub, 300);

        createTrackbar("area_ball_r_lb", "control", &config.area_ball_r_lb, 100);

        cv::createTrackbar("car1_Y_lb", "control2", &config.car1_Y_lb, 255);
        cv::createTrackbar("car1_Y_ub", "control2", &config.car1_Y_ub, 255);
        cv::createTrackbar("car1_U_lb", "control2", &config.car1_U_lb, 255);
        cv::createTrackbar("car1_U_ub", "control2", &config.car1_U_ub, 255);
        cv::createTrackbar("car1_V_lb", "control2", &config.car1_V_lb, 255);
        cv::createTrackbar("car1_V_ub", "control2", &config.car1_V_ub, 255);

        cv::createTrackbar("car2_Y_lb", "control2", &config.car2_Y_lb, 255);
        cv::createTrackbar("car2_Y_ub", "control2", &config.car2_Y_ub, 255);
        cv::createTrackbar("car2_U_lb", "control2", &config.car2_U_lb, 255);
        cv::createTrackbar("car2_U_ub", "control2", &config.car2_U_ub, 255);
        cv::createTrackbar("car2_V_lb", "control2", &config.car2_V_lb, 255);
        cv::createTrackbar("car2_V_ub", "control2", &config.car2_V_ub, 255);
    }
    else
    {
        destroyAllWindows();
    }

}
