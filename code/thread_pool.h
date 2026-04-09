#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>

class ThreadPool
{
public:
    explicit ThreadPool(size_t numThreads){
        for (size_t i = 0; i < numThreads; i++) {
            workers.emplace_back([this]{
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
                }
            });
        }
    };
    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
            
        }
    };
    void submit(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};