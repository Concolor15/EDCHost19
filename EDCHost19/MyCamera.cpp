#include "MyCamera.h"
#include "globalconfig.h"

#include <QtCore>
#include <QtDebug>

class MutexTryLocker {
  QMutex &m_;
  bool locked_;
public:
  MutexTryLocker(QMutex &m) : m_(m), locked_(m.tryLock()) {}
  ~MutexTryLocker() { if (locked_) m_.unlock(); }
  bool isLocked() const { return locked_; }
};

QVideoFrame MyFilterRunnable::run(
        QVideoFrame *input,
        const QVideoSurfaceFormat &surfaceFormat,
        RunFlags flags)
{
    if (input->map(QAbstractVideoBuffer::ReadOnly))
    {
        qCritical() << "Unable to map VideoFrame";
        goto ret;
    }

    input->unmap();

ret:
    return *input;
}

void ImgprocThread::run()
{
    MutexTryLocker lock(*inst_lock);

    if (!lock.isLocked())
    {
        //qError() << "multiple image processing thread are running!";
        return;
    }

    for (;;)
    {
        QMutexLocker _(cv_lock);
        cv->wait(cv_lock);

        void* p = param->fetchAndStoreOrdered(nullptr);
        if (p!=nullptr)
        {
            //TODO: update parameter
        }


    }
}
