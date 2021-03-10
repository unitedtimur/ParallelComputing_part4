#include "threadbarrier.h"

ThreadBarrier::ThreadBarrier(const size_t &threadCount) :
    m_threadCount(threadCount),
    m_threadCountWait(0),
    m_threadCountFree(0)
{}

void ThreadBarrier::wait() noexcept
{
    while (m_threadCountFree != 0);

    std::unique_lock<std::mutex> lock(m_mutex);

    ++m_threadCountWait;

    if (m_threadCountWait >= m_threadCount) {
        m_threadCountFree = m_threadCount - 1;
        m_threadCountWait = 0;
        m_cv.notify_all();
    }
    else {
        m_cv.wait(lock, [this]() {
            return m_threadCountWait == 0;
        });

        --m_threadCountFree;
    }
}
