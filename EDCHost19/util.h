#pragma once

#include <QtCore>
#include <opencv2/core/types.hpp>

template<typename T>
class AtomicData
{
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

inline QPointF point_cv2qt(cv::Point2f p)
{
    return {p.x, p.y};
}

class MutexTryLocker {
  QMutex &m_;
  bool locked_;
public:
  MutexTryLocker(QMutex &m) : m_(m), locked_(m.tryLock()) {}
  ~MutexTryLocker() { if (locked_) m_.unlock(); }
  bool isLocked() const { return locked_; }
};


