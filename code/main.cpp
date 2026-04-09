#include <iostream>
#include "thread_pool.h"

int main()
{
    ThreadPool pool(4);
    for (int i = 0; i < 6; ++i) {
        pool.submit([i]() {
            std::cout << "线程 " << std::this_thread::get_id() 
                      << " 正在执行任务 " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟耗时
        });
    }

    // main 函数睡个半秒，保证任务有时间被抢走
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "主函数结束，准备销毁线程池..." << std::endl;

    std::cout << "end" << std::endl;

}
