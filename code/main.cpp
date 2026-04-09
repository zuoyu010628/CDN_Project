#include <iostream>
#include "thread_pool.h"
#include "double_buffer.h"
void TestThreadPool()
{
    ThreadPool pool(4);
    for (int i = 0; i < 6; ++i)
    {
        pool.submit([i]()
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

void TestDoubleBuffer()
{
    DoubleBuffer db(100); // 故意弄个小一点的容量测试

    // 模拟业务线程疯狂写日志
    db.append("[INFO] 用户登录");
    db.append("[DEBUG] 查询数据库");

    std::cout << "=== 准备交接给后台线程 ===" << std::endl;

    // 模拟交接
    std::vector<char> data_to_write = db.swapBuffers();

    // 模拟后台线程把数据写入文件（这里直接打印出来看）
    std::string log_str(data_to_write.begin(), data_to_write.end());
    std::cout << "后台线程拿到数据: " << log_str << std::endl;

    // 交接后，业务线程继续写（其实已经写到 buffer2 里去了）
    db.append("[WARN] 缓存失效");

    // 再次交接
    std::vector<char> data_to_write2 = db.swapBuffers();
    std::string log_str2(data_to_write2.begin(), data_to_write2.end());
    std::cout << "后台线程拿到第二批数据: " << log_str2 << std::endl;

    db.append("[WARN] 缓存失效2");

    // 再次交接
    std::vector<char> data_to_write3 = db.swapBuffers();
    std::string log_str3(data_to_write3.begin(), data_to_write3.end());
    std::cout << "后台线程拿到第二批数据: " << log_str3 << std::endl;
}

int main()
{
    TestDoubleBuffer();
    std::cout << "end" << std::endl;
}
