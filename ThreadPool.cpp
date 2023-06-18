#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; i++) {
        workers.push_back(std::thread(std::bind(&ThreadPool::workerThread, this)));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::vector<std::thread>::iterator it = workers.begin(); it != workers.end(); ++it) {
        it->join();
    }
}

void ThreadPool::enqueueTask(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        Task task;
        task.func = job;
        taskQueue.push(task);
    }
    condition.notify_one();
}


void ThreadPool::workerThread() {
    while (true) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            while (!stop && taskQueue.empty()) {
                condition.wait(lock);
            }
            if (stop && taskQueue.empty()) {
                return;
            }
            task = taskQueue.front();
            taskQueue.pop();
        }
        task.func();
    }
}

// Memory management for files
// I/O Multiplexing
