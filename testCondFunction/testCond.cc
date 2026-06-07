#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <string>

// 多线程向显示器打印，数据会错乱
// void *Print(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while(true)
//     {
//         std::cout << "I am new thread: " << name << std::endl; 
//         // sleep(1);
//     }

//     return nullptr;
// }

// // 多线程向显示器打印，数据会错乱，使用锁保护
// void *Print(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while(true)
//     {
//         pthread_mutex_lock(&gmutex);
//         std::cout << "I am new thread: " << name << std::endl; 
//         pthread_mutex_unlock(&gmutex);
//         sleep(1);
//     }

//     return nullptr;
// }


pthread_mutex_t gmutex = PTHREAD_MUTEX_INITIALIZER;     // 定义一个全局锁
pthread_cond_t gcond = PTHREAD_COND_INITIALIZER;        // 定义一个条件变量

// 多线程向显示器打印，数据会错乱，使用锁保护
void *Print(void *argc)
{
    std::string name = static_cast<const char *>(argc);
    while(true)
    {
        pthread_mutex_lock(&gmutex);
        std::cout << "I am new thread: " << name << std::endl; 
        // 线程打印完毕后，在指定的条件变量下等待
        // int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex)
        pthread_cond_wait(&gcond, &gmutex);
        pthread_mutex_unlock(&gmutex);
        sleep(1);
    }

    return nullptr;
}

int main()
{
    pthread_t tid[4];       // 创建四个线程
    for(int i = 0; i < 4; i++)
    {
        // 给线程设置名字
        char *name = new char[64];
        snprintf(name, 64, "thread_%d", i);
        pthread_create(tid+i, nullptr, Print, (void*)name); // 创建四个线程
    }

    // 主线程按顺序的唤醒指定条件变量下等待的线程
    while(true)
    {
        // int pthread_cond_signal(pthread_cond_t *cond)
        // pthread_cond_signal(&gcond);
        // int pthread_cond_broadcast(pthread_cond_t *cond)
        pthread_cond_broadcast(&gcond);
        sleep(1);
    }


    for(int i = 0; i < 4; i++)
    {
        pthread_join(tid[i], nullptr);  // 等待线程
    }

    return 0;
}


