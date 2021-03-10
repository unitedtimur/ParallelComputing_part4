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
        std::lock_guard<std::mutex> guard(_mutexPrint);
        std::cout << this->str();
    }

private:
    static std::mutex _mutexPrint;
};

std::mutex ThreadSafePrint::_mutexPrint {};

#endif // THREADSAFEPRINT_H
