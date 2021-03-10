#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <ctime>
#include <random>
#include <string>
#include <atomic>
#include "threadSafePrint.h"

class barrier
{
    mutable size_t _threadsCount;
    size_t _threadsWaiting;
    size_t _threadsReleasing;
    std::condition_variable _cv;
    std::mutex _mutex;

public:
    barrier(const size_t &threadsAmount) :
        _threadsCount(threadsAmount),
        _threadsWaiting(0),
        _threadsReleasing(0)
    {}

    void wait()
    {
        while (_threadsReleasing != 0);

        std::unique_lock<std::mutex> lock(_mutex);

        if (++_threadsWaiting >= _threadsCount)
        {
            _threadsReleasing = _threadsCount - 1;
            _threadsWaiting = 0;
            _cv.notify_all();
        }
        else
        {
            _cv.wait(lock, [this]() { return _threadsWaiting == 0; });
            --_threadsReleasing;
        }
    }
};


int main()
{
    std::default_random_engine dre(std::time(nullptr));
    std::uniform_int_distribution<> uid(100, 1000);
    uint32_t thrAmount;

    std::cout << "Enter threads amount: ";
    std::cin >> thrAmount;

    barrier br(thrAmount);
    std::vector<std::thread> threads;
    std::atomic_uint32_t released(0);

    for (size_t i = 0; i < thrAmount; ++i)
    {
        threads.emplace_back([&]()
            {
                for (int32_t j = 0; j < 4; ++j)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(uid(dre)));
                    br.wait();
                    ThreadSafePrint{} << std::this_thread::get_id() << " released\n";
                    if (++released % thrAmount == 0)
                        sPrint << std::endl;
                }
            });
    }

    for (auto& thr : threads)
        if (thr.joinable())
        {
            auto id = thr.get_id();
            thr.join();
            sPrint << "id = " << id << " joined" << std::endl;
        }

    return 0;
}
