#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>

class AsyncLogger
{
public:
    explicit AsyncLogger(size_t capacity = 4096);
    ~AsyncLogger();
    void Start();
    void Stop();
    void Append(const std::string &log);
    AsyncLogger(const AsyncLogger &) = delete;
    AsyncLogger &operator=(const AsyncLogger &) = delete;
private:
    void LoggerFunc();

    size_t capacity;
    std::vector<char> currenBuffer;
    std::vector<std::vector<char>> pendingBuffer;
    bool running = false;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread loggerThread;
};