#ifndef THREADSAFEPRINT_H
#define THREADSAFEPRINT_H

#include <sstream>
#include <mutex>
#include <iostream>

#define sPrint ThreadSafePrint{}

class ThreadSafePrint final : public std::ostringstream
{
public:
    explicit ThreadSafePrint() = default;

    ~ThreadSafePrint() noexcept
    {
        std::lock_guard<std::mutex> guard(m_mutexPrint);
        std::cout << this->str();
    }

private:
    static std::mutex m_mutexPrint;
};

std::mutex ThreadSafePrint::m_mutexPrint {};

#endif // THREADSAFEPRINT_H
