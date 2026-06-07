#include "BlockQueue.hpp"
#include "Task.hpp"
#include <time.h>
#include <stdlib.h>

using namespace AY; // 展开命名空间，"Thread.hpp"中的

int gnum = 0;
pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER; // 保护 gnum

int GetNumber()
{
    pthread_mutex_lock(&glock);
    int number = gnum++;
    pthread_mutex_unlock(&glock);

    return number;
}

// 多生产者多消费者
int main()
{
    srand((unsigned int)time(nullptr));
    std::unique_ptr<BlockQueue<int>> bq = std::make_unique<BlockQueue<int> >();
    Thread consumer([&bq](){
        std::string con_name = "Consumer_" + std::to_string(GetNumber()); // 设置名字
        // int pthread_setname_np(pthread_t thread, const char *name);
        // 设置消费者线程的名称，长度不能超过16
        pthread_setname_np(pthread_self(), con_name.c_str());

        // 消费者不断消费
        while (true)
        {
            Task t;
            // 1. 取数据
            bq->PopQueue(&t);
            // 2. 处理数据
            std::cout << con_name << " 消费: ";
            t.Print();
        }
    });

    Thread productor([&bq]() {
        std::string pro_name = "Productor_" + std::to_string(GetNumber()); // 设置名字
        // int pthread_setname_np(pthread_t thread, const char *name);
        // 设置消费者线程的名称，长度不能超过16
        pthread_setname_np(pthread_self(), pro_name.c_str());

        // 生产者不断的生产
        while (true)
        {
            // 1. 获取数据
            int datax = rand() % 100 + 1;
            usleep(2);
            int datay = rand() % 100 + 1;

            // 2. 生产数据
            Task t(datax, datay);
            bq->PushQueue(t);
            std::cout << pro_name << " 生产: ";
            t.Print();
        }
    });

    consumer.StartThread();     // 创建线程
    productor.StartThread();    // 创建线程

    consumer.JoinThread();
    productor.JoinThread();

    return 0;
}



// // 多生产者多消费者
// int main()
// {
//     std::unique_ptr<BlockQueue<int>> bq = std::make_unique<BlockQueue<int> >();
//     Thread consumer([&bq](){
//         std::string con_name = "Consumer_" + std::to_string(GetNumber()); // 设置名字
//         // int pthread_setname_np(pthread_t thread, const char *name);
//         // 设置消费者线程的名称，长度不能超过16
//         pthread_setname_np(pthread_self(), con_name.c_str());

//         // 消费者不断消费
//         while (true)
//         {
//             // sleep(5);
//             int outdata;
//             bq->PopQueue(&outdata);
//             std::cout << con_name << " 消费: " << outdata << std::endl;
//         }
//     });

//     Thread productor([&bq]() {
//         std::string pro_name = "Productor_" + std::to_string(GetNumber()); // 设置名字
//         // int pthread_setname_np(pthread_t thread, const char *name);
//         // 设置消费者线程的名称，长度不能超过16
//         pthread_setname_np(pthread_self(), pro_name.c_str());

//         int indata = 1; // 投入的数据

//         // 生产者不断的生产
//         while (true)
//         {
//             // sleep(5);
//             bq->PushQueue(indata);
//             // std::cout << "productor: " << indata << std::endl;
//             std::cout << pro_name << " 生产: " << indata << std::endl;
//             indata++;
//         }
//     });

//     consumer.StartThread();     // 创建线程
//     productor.StartThread();    // 创建线程

//     consumer.JoinThread();
//     productor.JoinThread();

//     return 0;
// }