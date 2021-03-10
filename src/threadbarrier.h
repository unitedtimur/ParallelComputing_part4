#ifndef THREADBARRIER_H
#define THREADBARRIER_H

#include <mutex>
#include <condition_variable>

class ThreadBarrier final
{
public:
    explicit ThreadBarrier(const size_t &threadCount);

    void wait() noexcept;

private:
    mutable size_t m_threadCount;
    size_t m_threadCountWait;
    size_t m_threadCountFree;

    mutable std::condition_variable m_cv;
    std::mutex m_mutex;
};

#endif // THREADBARRIER_H
