#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

struct Task {
    std::function<void()> func;
};

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void enqueueTask(const std::function<void()>& job);

private:
    std::vector<std::thread> workers;
    std::queue<Task> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

    void workerThread();
};
