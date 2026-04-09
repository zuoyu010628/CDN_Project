#include <iostream>
#include <vector>
#include <string>
#include <memory>

class DoubleBuffer
{
public:
    explicit DoubleBuffer(size_t capacity = 4096) : capacity_(capacity)
    {
        buffer1_.reserve(capacity_);
        buffer2_.reserve(capacity_);
        current_ = std::make_unique<std::vector<char>>();
        next_ = std::make_unique<std::vector<char>>();
    }

    void append(const std::string &log)
    {
        if (current_->size() + log.size() > capacity_)
        {
            std::cout << "buffer out, swap buffer" << std::endl;
            return;
        }
        current_->insert(current_->end(), log.begin(), log.end());
    }

    std::vector<char> swapBuffers()
    {
        std::swap(current_, next_);
        std::vector<char> fullBuffer = std::move(*next_);
        next_->reserve(capacity_);
        next_->clear();
        return fullBuffer;
    }
    DoubleBuffer(const DoubleBuffer &) = delete;
    DoubleBuffer &operator=(const DoubleBuffer &) = delete;

private:
    size_t capacity_;
    std::vector<char> buffer1_;
    std::vector<char> buffer2_;
    std::unique_ptr<std::vector<char>> current_;
    std::unique_ptr<std::vector<char>> next_;
    // std::vector<char>* current_;
    // std::vector<char>* next_;
};