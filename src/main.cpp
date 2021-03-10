#include <iostream>
#include <vector>
#include <thread>
#include <random>

#include "threadsafeprint.h"
#include "threadbarrier.h"

std::default_random_engine DRE(std::time(nullptr));
std::uniform_int_distribution<> UID(0, 1000);

auto task() -> void;

int main()
{
    size_t threadCount;

    sPrint << "Enter count of threads: ";
    std::cin >> threadCount;

    ThreadBarrier br(threadCount);
    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    for (size_t i = 0; i < threadCount; ++i)
        threads.emplace_back([&]() {
            for (int32_t j = 0; j < 4; ++j)
            {
                task();
                br.wait();

                sPrint << std::this_thread::get_id() << " has been ended..." << std::endl;
            }
        });

    for (auto& thr : threads)
        if (thr.joinable()) {
            const std::thread::id id = thr.get_id();

            thr.join();

            sPrint << id << " has been joined..." << std::endl;
        }

    return 0;
}

auto task() -> void
{
    // Типо задача тут
    std::this_thread::sleep_for(std::chrono::milliseconds(UID(DRE)));
}
