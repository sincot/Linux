#include <iostream>
#include <pthread.h>
#include <unistd.h>

// int gticket = 10000; // 一共 10000 张票，共享资源

// void *Routine(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while (1)
//     {
//         if (gticket > 0)    // 进入到该判断语句中时，就已经在抢票了
//         {
//             usleep(1000);   // 抢票所花的时间
//             std::cout << name << " sells ticket: " << gticket << std::endl;
//             gticket--;
//         }
//         else { break; }
//     }

//     return nullptr;
// }

// int main()
// {
//     pthread_t tid1, tid2, tid3, tid4;
//     pthread_create(&tid1, nullptr, Routine, (void *)"thread_1");
//     pthread_create(&tid2, nullptr, Routine, (void *)"thread_2");
//     pthread_create(&tid3, nullptr, Routine, (void *)"thread_3");
//     pthread_create(&tid4, nullptr, Routine, (void *)"thread_4");

//     pthread_join(tid1, nullptr);
//     pthread_join(tid2, nullptr);
//     pthread_join(tid3, nullptr);
//     pthread_join(tid4, nullptr);

//     return 0;
// }


// int gticket = 10000; // 一共 10000 张票，共享资源

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 初始化锁
// void *Routine(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while (1)
//     {
//         // 竞争锁失败的线程，会阻塞等待锁资源
//         pthread_mutex_lock(&mutex);     // 加锁
//         if (gticket > 0)    // 进入到该判断语句中时，就已经在抢票了
//         {
//             usleep(1000);   // 抢票所花的时间
//             std::cout << name << " sells ticket: " << gticket << std::endl;
//             gticket--;

//             pthread_mutex_unlock(&mutex);   // 解锁
//         }
//         else 
//         { 
//             pthread_mutex_unlock(&mutex);   // 解锁
//             break; 
//         }
//     }

//     return nullptr;
// }

// int main()
// {
//     pthread_t tid1, tid2, tid3, tid4;
//     pthread_create(&tid1, nullptr, Routine, (void *)"thread_1");
//     pthread_create(&tid2, nullptr, Routine, (void *)"thread_2");
//     pthread_create(&tid3, nullptr, Routine, (void *)"thread_3");
//     pthread_create(&tid4, nullptr, Routine, (void *)"thread_4");

//     pthread_join(tid1, nullptr);
//     pthread_join(tid2, nullptr);
//     pthread_join(tid3, nullptr);
//     pthread_join(tid4, nullptr);

//     return 0;
// }


// int gticket = 10000; // 一共 10000 张票，共享资源

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 初始化锁
// void *Routine1(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while (1)
//     {
//         // 竞争锁失败的线程，会阻塞等待锁资源
//         pthread_mutex_lock(&mutex);     // 加锁
//         if (gticket > 0)    // 进入到该判断语句中时，就已经在抢票了
//         {
//             usleep(1000);   // 抢票所花的时间
//             std::cout << name << " sells ticket: " << gticket << std::endl;
//             gticket--;

//             pthread_mutex_unlock(&mutex);   // 解锁
//         }
//         else 
//         { 
//             pthread_mutex_unlock(&mutex);   // 解锁
//             break; 
//         }
//     }

//     return nullptr;
// }

// void *Routine2(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while (1)
//     {
//         if (gticket > 0)    // 进入到该判断语句中时，就已经在抢票了
//         {
//             usleep(1000);   // 抢票所花的时间
//             std::cout << name << " sells ticket: " << gticket << std::endl;
//             gticket--;
//         }
//         else 
//         { 
//             break; 
//         }
//     }

//     return nullptr;
// }

// int main()
// {
//     pthread_t tid1, tid2, tid3, tid4;
//     pthread_create(&tid1, nullptr, Routine1, (void *)"thread_1");
//     pthread_create(&tid2, nullptr, Routine1, (void *)"thread_2");
//     pthread_create(&tid3, nullptr, Routine2, (void *)"thread_3");
//     pthread_create(&tid4, nullptr, Routine2, (void *)"thread_4");

//     pthread_join(tid1, nullptr);
//     pthread_join(tid2, nullptr);
//     pthread_join(tid3, nullptr);
//     pthread_join(tid4, nullptr);

//     return 0;
// }


int gticket = 10000; // 一共 10000 张票，共享资源

class thread_data
{
public:
    thread_data(const std::string &name, pthread_mutex_t *mutex)
        : _name(name), _mutex(mutex)
    {}

public:
    std::string _name;
    pthread_mutex_t *_mutex;
};

void *Routine(void *argc)
{
    thread_data* td = static_cast<thread_data *>(argc);
    while (1)
    {
        // 竞争锁失败的线程，会阻塞等待锁资源
        pthread_mutex_lock(td->_mutex);
        if (gticket > 0)    // 进入到该判断语句中时，就已经在抢票了
        {
            usleep(1000);   // 抢票所花的时间
            std::cout << td->_name << " sells ticket: " << gticket << std::endl;
            gticket--;
            
            pthread_mutex_unlock(td->_mutex);
        }
        else
        {
            pthread_mutex_unlock(td->_mutex);
            break; 
        }
    }

    return nullptr;
}

int main()
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    pthread_t tid1, tid2, tid3, tid4;
    thread_data td1("thread_1", &mutex), td2("thread_2", &mutex), td3("thread_3", &mutex), td4("thread_4", &mutex);

    pthread_create(&tid1, nullptr, Routine, (void*)&td1);
    pthread_create(&tid2, nullptr, Routine, (void*)&td2);
    pthread_create(&tid3, nullptr, Routine, (void*)&td3);
    pthread_create(&tid4, nullptr, Routine, (void*)&td4);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
    pthread_join(tid3, nullptr);
    pthread_join(tid4, nullptr);

    pthread_mutex_destroy(&mutex);

    return 0;
}


