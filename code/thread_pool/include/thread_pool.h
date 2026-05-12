#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>

class ThreadPool
{
public:
    explicit ThreadPool(size_t numThreads);
    ~ThreadPool();
    void Submit(std::function<void()> task);
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};