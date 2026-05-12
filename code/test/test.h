#include <iostream>
#include "thread_pool.h"
#include "async_logger.h"

void TestThreadPool()
{
    ThreadPool pool(4);
    for (int i = 0; i < 6; ++i)
    {
        pool.Submit([i]()
                    {
                        std::cout << "线程 " << std::this_thread::get_id()
                                  << " 正在执行任务 " << i << std::endl;
                        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟耗时
                    });
    }

    // main 函数睡个半秒，保证任务有时间被抢走
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "主函数结束，准备销毁线程池..." << std::endl;
}

void TestLogger()
{
    AsyncLogger logger(400); // 故意设置很小的缓冲区（100字节），极易触发 swap

    logger.Start(); // 拉起后台线程

    // 模拟 5 个并发业务线程
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back([&logger, i]()
                             {
            for (int j = 0; j < 3; ++j) {
                // 拼接一个超过 50 字节的字符串
                std::string log = "[Thread " + std::to_string(i) + "] 执行任务 " + std::to_string(j) + "，这是一条长日志数据...";
                logger.Append(log);
            } });
    }

    // 等待所有业务线程打完日志
    for (auto &t : threads)
    {
        t.join();
    }

    std::cout << "=== 业务线程全部结束，准备销毁日志系统 ===" << std::endl;
}
