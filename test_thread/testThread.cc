#include <iostream>
#include <cstdio>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include "Task.hpp"
#include <time.h>

// new thread 执行流执行的任务
// void *Routine(void *argc)
// {
//     std::string name = static_cast<const char*>(argc);
//     while(true)
//     {
//         // std::cout << "I am new thread" << std::endl;
//         // printf("I am new thread, tid: 0x%lx\n", pthread_self());
//         printf("I am new thread, tid: 0x%lx, pid: %d\n", pthread_self(), getpid());
//         sleep(1);
//     }
// }

// 创建单线程
// int main()
// {
//     // int pthread_create(pthread_t *restrict thread,
//     //                    const pthread_attr_t *restrict attr,
//     //                    void *(*start_routine)(void *),
//     //                    void *restrict arg);
//     pthread_t tid;
//     // Routine 函数的参数为 “thread_1”,将字符串的起始地址传递给了 Routine 函数
//     int n = pthread_create(&tid, nullptr, Routine, (void*)"thread_1");

//     // std::cout << "new thread id: " << tid << std::endl;
//     // printf("new thread id: %ld\n", tid);    // 以十进制的形式打印
//     printf("new thread id: 0x%lx\n", tid);    // 以十六进制的形式打印

//     // main thread 执行流执行的任务
//     while(true)
//     {
//         // std::cout << "I am main thread ..." << std::endl;
//         // printf("I am new thread, tid: 0x%lx\n", pthread_self());
//         printf("I am main thread, tid: 0x%lx, pid: %d\n", pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }

// // new thread 执行流执行的任务
// void *Routine(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while (true)
//     {
//         sleep(10);
//         // std::cout << "I am new thread" << std::endl;
//         // printf("I am new thread, tid: 0x%lx\n", pthread_self());
//         printf("I am new thread: %s, tid: 0x%lx, pid: %d\n", name.c_str(), pthread_self(), getpid());
//         std::cout << std::endl;
//         sleep(1);
//     }
// }

// // 创建多线程
// int main()
// {
//     const int num = 10;
//     for (int i = 0; i < num; i++)
//     {
//         pthread_t tid;
//         char thread_name[64];
//         snprintf(thread_name, sizeof(thread_name), "thread_%d", i); // 构建线程名
//         pthread_create(&tid, nullptr, Routine, thread_name);
//         sleep(1);
//     }

//     // 接下来的工作都是主线程在执行
//     while (true)
//     {
//         // printf("I am main thread, tid: 0x%lx, pid: %d\n", pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }

// g_val 值的修改
// int g_val = 10;

// // new thread 执行流执行的任务
// void *Routine(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while (true)
//     {
//         // sleep(10);
//         // std::cout << "I am new thread" << std::endl;
//         // printf("I am new thread, tid: 0x%lx\n", pthread_self());
//         printf("I am new thread: %s, tid: 0x%lx, pid: %d, g_val: %d, &g_val: %p\n", \
//             name.c_str(), pthread_self(), getpid(), g_val, &g_val);
//         std::cout << std::endl;
//         sleep(1);
//         g_val++;
//     }
// }

// // 创建多线程
// int main()
// {
//     const int num = 1;
//     for (int i = 0; i < num; i++)
//     {
//         pthread_t tid;
//         char thread_name[64];
//         snprintf(thread_name, sizeof(thread_name), "thread_%d", i); // 构建线程名
//         pthread_create(&tid, nullptr, Routine, thread_name);
//         sleep(1);
//     }

//     // 接下来的工作都是主线程在执行
//     while (true)
//     {
//         printf("I am main thread, tid: 0x%lx, pid: %d, g_val: %d, &g_val: %p\n", \
//                 pthread_self(), getpid(), g_val, &g_val);
//         sleep(1);
//     }

//     return 0;
// }


// 主线程 val 值的修改

// int *addr = nullptr;    // 指向 val 值

// // new thread 执行流执行的任务
// void *Routine(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while (true)
//     {
//         // sleep(10);
//         // std::cout << "I am new thread" << std::endl;
//         // printf("I am new thread, tid: 0x%lx\n", pthread_self());
//         printf("I am new thread: %s, tid: 0x%lx, pid: %d, *addr: %d, addr: %p\n", \
//             name.c_str(), pthread_self(), getpid(), *addr, addr);
//         std::cout << std::endl;
//         sleep(1);
//         *addr += 1;
//     }
// }

// // 创建多线程
// int main()
// {
//     int val = 10;
//     addr = &val;
//     const int num = 1;
//     for (int i = 0; i < num; i++)
//     {
//         pthread_t tid;
//         char thread_name[64];
//         snprintf(thread_name, sizeof(thread_name), "thread_%d", i); // 构建线程名
//         pthread_create(&tid, nullptr, Routine, thread_name);
//         sleep(1);
//     }

//     // 接下来的工作都是主线程在执行
//     while (true)
//     {
//         printf("I am main thread, tid: 0x%lx, pid: %d, val: %d, &val: %p\n", \
//                 pthread_self(), getpid(), val, &val);
//         sleep(1);
//     }

//     return 0;
// }


// 多线程的异常问题

// 问题1：除0异常
// // new thread 执行流执行的任务
// void *Routine(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     while (true)
//     {
//         // sleep(10);
//         // std::cout << "I am new thread" << std::endl;
//         // printf("I am new thread, tid: 0x%lx\n", pthread_self());
//         printf("I am new thread: %s, tid: 0x%lx, pid: %d\n", \
//             name.c_str(), pthread_self(), getpid());
//         std::cout << std::endl;
//         sleep(1);

//         if(name == "thread_5")
//         {
//             std::cout << "thread_5 线程出现异常了" << std::endl;
//             // 设置除0错误
//             int a = 10;
//             a /= 0;
//         }
//     }
// }

// // 创建多线程
// int main()
// {
//     const int num = 10;
//     for (int i = 0; i < num; i++)
//     {
//         pthread_t tid;
//         char thread_name[64];
//         snprintf(thread_name, sizeof(thread_name), "thread_%d", i); // 构建线程名
//         pthread_create(&tid, nullptr, Routine, thread_name);
//         sleep(1);
//     }

//     // 接下来的工作都是主线程在执行
//     while (true)
//     {
//         printf("I am main thread, tid: 0x%lx, pid: %d,\n", \
//                 pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }


// 问题2：因为并发问题，导致的我呢提
// // new thread 执行流执行的任务
// void *Routine(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     printf("I am new thread: %s, tid: 0x%lx, pid: %d\n", \
//             name.c_str(), pthread_self(), getpid());

//     while (true)
//     {
//         sleep(1);
//     }
// }

// // 创建多线程
// int main()
// {
//     const int num = 10;
//     for (int i = 0; i < num; i++)
//     {
//         pthread_t tid;
//         char thread_name[64];
//         snprintf(thread_name, sizeof(thread_name), "thread_%d", i); // 构建线程名
//         pthread_create(&tid, nullptr, Routine, thread_name);
//         // sleep(1);
//     }

//     // 接下来的工作都是主线程在执行
//     while (true)
//     {
//         printf("I am main thread, tid: 0x%lx, pid: %d\n", \
//                 pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }

// 解决方案
// new thread 执行流执行的任务
// void *Routine(void *argc)
// {
//     std::string name = static_cast<const char *>(argc);
//     printf("I am new thread: %s, tid: 0x%lx, pid: %d\n", \
//             name.c_str(), pthread_self(), getpid());
            
//     while (true)
//     {
//         sleep(1);
//     }
// }

// // 创建多线程
// int main()
// {
//     const int num = 10;
//     for (int i = 0; i < num; i++)
//     {
//         pthread_t tid;
//         char *thread_name = new char[64];

//         /******************* 修改点 *************************/
//         sprintf(thread_name, "thread_%d", i); // 构建线程名
//         pthread_create(&tid, nullptr, Routine, thread_name);

//         sleep(1);
//     }

//     // 接下来的工作都是主线程在执行
//     while (true)
//     {
//         printf("I am main thread, tid: 0x%lx, pid: %d\n", \
//                 pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }


// 执行任务
// void *Routine(void *argc)
// {
//     Task *task = static_cast<Task *>(argc);
//     task->add();
//     std::cout << task->RuturnResult() << std::endl;

//     return nullptr;
// }

// // 创建多线程
// int main()
// {
//     srand((unsigned int)time(nullptr));
//     const int num = 10;
//     for (int i = 0; i < num; i++)
//     {
//         pthread_t tid;
//         int x = rand() % 10 + 1;    // 传参值 x
//         sleep(1);   // 让生成的随机数更随机
//         int y = rand() % 5 + 1;     // 传参值 y

//         Task *task = new Task(x, y);
//         pthread_create(&tid, nullptr, Routine, task);

//         sleep(1);
//     }

//     // 接下来的工作都是主线程在执行
//     while (true)
//     {
//         printf("I am main thread, tid: 0x%lx, pid: %d\n", \
//                 pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }

// 终止进程

// void *Routine(void *argc)
// {
//     Task *task = static_cast<Task *>(argc);
//     task->add();
//     std::cout << task->RuturnResult() << std::endl;

//     while(true)
//     {
//         pthread_exit(nullptr);
//     }

//     return nullptr;
// }

// // 创建多线程
// int main()
// {
//     srand((unsigned int)time(nullptr));
//     const int num = 10;
//     for (int i = 0; i < num; i++)
//     {
//         pthread_t tid;
//         int x = rand() % 10 + 1;    // 传参值 x
//         sleep(1);   // 让生成的随机数更随机
//         int y = rand() % 5 + 1;     // 传参值 y

//         Task *task = new Task(x, y);
//         pthread_create(&tid, nullptr, Routine, task);

//         sleep(1);
//     }

//     // 接下来的工作都是主线程在执行
//     while (true)
//     {
//         printf("I am main thread, tid: 0x%lx, pid: %d\n", \
//                 pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }


// 调用 pthread_join 函数 —— 主线程比新线程先退出
// void* Routine(void* argc)
// {
//     std::string name = static_cast<const char*>(argc);
//     while(true)
//     {
//         std::cout << "new thread " << name << std::endl;
//         sleep(1);
//         break;
//     }

//     return (void*)1;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, Routine, (void*)"thread_1");

//     // 1. 回收等待新线程退出，如果不等待会导致类似僵尸进程的问题
//     // 2. 获取新线程的执行结果
//     void *retval = nullptr;     // 8字节大小
//     int n = pthread_join(tid, &retval); 
//     if(n == 0)
//     {
//         // retval 到底是多少
//         std::cout << "join sucess " << (long long)retval << std::endl;
//     }

//     return 0;
// }