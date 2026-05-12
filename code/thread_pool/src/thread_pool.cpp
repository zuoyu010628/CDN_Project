#include "thread_pool.h"

ThreadPool::ThreadPool(size_t numThreads)
{
    for (size_t i = 0; i < numThreads; i++)
    {
        workers.emplace_back([this]
                             {
                while (true){
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        while (this->tasks.empty() && !this->stop) {
                            this->condition.wait(lock);
                        }
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }
                        task = this->tasks.front();
                        this->tasks.pop();
                    }
                    if (task != nullptr) {
                        task();
                    }
                } });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (auto &worker : workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

void ThreadPool::Submit(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
    }
    condition.notify_one();
}