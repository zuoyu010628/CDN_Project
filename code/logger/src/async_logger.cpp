#include <iostream>
#include "async_logger.h"
AsyncLogger::AsyncLogger(size_t capacity) : capacity(capacity)
{
    currenBuffer.reserve(capacity);
}

AsyncLogger::~AsyncLogger()
{
    Stop();
}

void AsyncLogger::Start()
{
    running = true;
    loggerThread = std::thread(&AsyncLogger::LoggerFunc, this);
}

void AsyncLogger::Stop()
{
    if (!running)
    {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
    }
    cv.notify_one();
    if (loggerThread.joinable())
    {
        loggerThread.join();
    }
}

void AsyncLogger::Append(const std::string &log)
{
    std::lock_guard<std::mutex> lock(mtx);
    size_t totalLen = log.size() + 1;
    if (currenBuffer.size() + totalLen > capacity)
    {
        pendingBuffer.emplace_back(currenBuffer);
        currenBuffer = std::vector<char>();
        currenBuffer.reserve(capacity);
        cv.notify_one();
    }
    currenBuffer.insert(currenBuffer.end(), log.begin(), log.end());
    currenBuffer.push_back('\n');
}
void AsyncLogger::LoggerFunc()
{
    while (running)
    {
        std::vector<std::vector<char>> writeBuffer;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]()
                    { return !running || !pendingBuffer.empty(); });
            writeBuffer.swap(pendingBuffer);
        }
        std::cout << "[Logger] " << std::endl;
        for (auto &buf : writeBuffer)
        {
            std::string str(buf.begin(), buf.end());
            std::cout << str << std::endl;
        }
    }
    std::vector<char> tail;
    {
        std::lock_guard<std::mutex> lock(mtx);
        tail.swap(currenBuffer);
    }
    if (!tail.empty())
    {
        std::cout << "[end] " << std::endl;
        std::string str(tail.begin(), tail.end());
        std::cout << str << std::endl;
    }
}