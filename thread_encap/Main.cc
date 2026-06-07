#include "Thread.hpp"

// void Task()
// {
//     char name[64];
//     // int pthread_getname_np(pthread_t thread, char name[.size], size_t size)
//     pthread_getname_np(pthread_self(), name, sizeof(name));

//     // int cnt = 10;
//     // while(cnt)
//     // {
//     //     std::cout << "new thread running" << name << std::endl;
//     //     sleep(1);
//     //     cnt--;
//     // }


//     while(true)
//     {
//         std::cout << "new thread running" << name << std::endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     AY::Thread t(Task);
//     t.StartThread();
    
//     t.DetachThread();   // 分离线程

//     // sleep(10000);   // 主线程不退出
//     sleep(5);   // 先休眠5秒

//     t.StopThread();     // 终止新线程

//     sleep(1);   // 再休眠1秒

//     t.JoinThread();     // 等待线程

//     t.PrintInfo();

//     return 0;
// }


// 多线程
// void Task()
// {
//     char name[64];
//     // int pthread_getname_np(pthread_t thread, char name[.size], size_t size)
//     pthread_getname_np(pthread_self(), name, sizeof(name));

//     int cnt = 5;
//     while(cnt--)
//     {
//         std::cout << "new thread running: " << name << " cnt: " << cnt << std::endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     // 使用 vector 容器将线程管理起来
//     std::vector<AY::Thread> threads;

//     const int num = 10;     // 创建 10 个线程
//     for(int i = 0; i < num; i++)
//     {
//         // 将这些线程保存到 threads 对象中
//         threads.emplace_back(Task);
//     }

//     for(auto &thread : threads)
//     {
//         // 创建线程
//         thread.StartThread();
//         sleep(1);
//     }

//     for(auto &thread : threads)
//     {
//         // 等待线程
//         thread.JoinThread();    
//     }

//     return 0;
// }


// version1:
// void Task(int cnt)
// {
//     char name[64];
//     // int pthread_getname_np(pthread_t thread, char name[.size], size_t size)
//     pthread_getname_np(pthread_self(), name, sizeof(name));

//     while(cnt--)
//     {
//         std::cout << "new thread running: " << name << " cnt: " << cnt << std::endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     // 使用 vector 容器将线程管理起来
//     std::vector<AY::Thread<int>> threads;

//     const int num = 10;     // 创建 10 个线程
//     for(int i = 0; i < num; i++)
//     {
//         // 将这些线程保存到 threads 对象中
//         // 给线程传递参数，循环5次
//         threads.emplace_back(Task, 5);
//     }

//     for(auto &thread : threads)
//     {
//         // 创建线程
//         thread.StartThread();
//         sleep(1);
//     }

//     for(auto &thread : threads)
//     {
//         // 等待线程
//         thread.JoinThread();    
//     }

//     return 0;
// }



// version2: 多线程
// class Task
// {
// public:
//     Task(int x, int y) :_x(x), _y(y)
//     {}

//     void Add() { _result = _x + _y; }

//     void Print() { std::cout << _x << " + " << _y << " = " << _result << std::endl; }

// private:
//     int _x;
//     int _y;
//     int _result;
// };


// int main()
// {
//     // 构建任务
//     srand((unsigned int)time(nullptr));
//     const int num = 10;     // 创建 10 个线程和任务
//     std::vector<Task> tasks;
//     for(int i = 0; i < num; i++)
//     {
//         tasks.emplace_back(rand()%10+1, rand()%100+1);
//     }


//     // 使用 vector 容器将线程管理起来
//     std::vector<AY::Thread> threads;
//     for(int i = 0; i < num; i++)
//     {
//         // 将这些线程保存到 threads 对象中
//         // 将线程模块和任务模块结合起来
//         threads.emplace_back([i, &tasks](){
//             tasks[i].Add();
//         });
//     }

//     for(auto &thread : threads)
//     {
//         // 创建线程
//         thread.StartThread();
//         // sleep(1);
//     }

//     for(auto &thread : threads)
//     {
//         // 等待线程
//         thread.JoinThread();
//         thread.PrintInfo();   
//     }

//     for(auto& task : tasks)
//     {
//         task.Print();
//     }

//     return 0;
// }

// version2: 单线程
// class Task
// {
// public:
//     Task(int x, int y) :_x(x), _y(y)
//     {}

//     void Add() { _result = _x + _y; }

//     void Print() { std::cout << _x << " + " << _y << " = " << _result << std::endl; }

// private:
//     int _x;
//     int _y;
//     int _result;
// };


// int main()
// {
//     srand((unsigned int)time(nullptr));

//     Task task(rand()%10+1, rand()%100+1);   // 任务类
//     AY::Thread thread([&task]()->void{
//         task.Add();
//     });    // 线程类

//     thread.StartThread();   // 创建线程
//     thread.JoinThread();    // 等待线程

//     task.Print();   // 打印任务的执行结果

//     return 0;
// }
