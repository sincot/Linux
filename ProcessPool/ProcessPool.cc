#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <cstdio>
#include <functional>
#include <sys/wait.h>

//////////////////////////////// 子进程要完成的任务 ///////////////////////////////////////////
void function1()
{
    std::cout << "function1" << std::endl;
    sleep(1);
}

void function2()
{
    std::cout << "function2" << std::endl;
    sleep(1);
}

void function3()
{
    std::cout << "function3" << std::endl;
    sleep(1);
}

void function4()
{
    std::cout << "function4" << std::endl;
    sleep(1);
}

void function5()
{
    std::cout << "function5" << std::endl;
    sleep(1);
}

typedef void (*task_t)(); // 使用函数指针

// 将上述的5个任务保存在任务数组中(全局变量)
std::vector<task_t> tasks = {function1, function2, function3, function4, function5};

////////////////////////////////////// 进程池 ////////////////////////////////////////////////
// 定义常量列表
enum
{
    NO_PROBLEM = 0,    // 没有问题
    PIPE_CREATE_ERROR, // 管道创建失败
    FORK_CREATE_ERROR  // 子进程创建失败
};

const int processNum = 5; // 定义创建子进程的数量

// 类型的重命名 —— 将 function<void (int)> 类型重命名为 task_t
// typedef std::function<void (int)> task_t;
// using task_t = std::function<void(int)>;
using cb_t = std::function<void(int)>;

// 子进程的入口函数
void Task(int fd)
{
    while (true)
    {
        int task_code = 0; // 获取到的任务码
        // 从指定的管道读端读取数据
        ssize_t n = read(fd, &task_code, sizeof(task_code));
        // 返回值的大小等于任务码的大小，即读取到的数据的长度等于任务码的长度就可以说明获取到了任务码
        if (n == sizeof(task_code))
        {
            if (task_code >= 0 && task_code <= tasks.size()) // 检测任务码是否安全
            {
                // 根据任务码选择对应的任务,并执行任务表中的任务
                tasks[task_code]();
            }
        }
        // 子进程读到0,即读到了管道的结尾，即父进程关闭了管道的写端,子进程需要退出了
        else if (n == 0)
        {
            std::cout << getpid() << " quit ..." << std::endl; // 显示哪个子进程退出了
            break;
        }
        else
        {
            perror("read"); // 读取错误
            break;
        }
    }
}

// // 定义管道类
// class Channel
// {
// public:
//     Channel(int wfd, pid_t pid) // 构造函数
//         : _wfd(wfd), _child_pid(pid)
//     {
//         // 自定义实现子进程的名字，to_string 函数将整型转化成字符串
//         _child_name = "child_name-" + std::to_string(_child_pid);
//     }
//     ~Channel() {} // 析构函数

//     void PrintInfo()
//     {
//         printf("wfd: %d, child_pid: %d, child_name: %s\n", _wfd, _child_pid, _child_name.c_str());
//     }

// private:
//     // 成员属性
//     int _wfd;                // 写文件描述符
//     pid_t _child_pid;        // 对应的子进程的pid
//     std::string _child_name; // 对应的子进程的名称
// };

// void CreatProcessAndChannel()
// {
//     // 1. 创建多个进程和管道
//     for (int i = 0; i < processNum; i++)
//     {
//         // 1. 创建多个管道
//         int pipefd[2] = {0};  // 文件描述符数组
//         int n = pipe(pipefd); // 创建管道
//         if (n < 0)            // 创建管道失败
//         {
//             std::cerr << "pipe create error" << std::endl; // 使用C++中打印错误的方式
//             exit(PIPE_CREATE_ERROR);
//         }

//         // 创建管道成功，创建子进程
//         pid_t id = fork();
//         if (id < 0) // 创建子进程失败
//         {
//             std::cerr << "chile process create error" << std::endl; // 使用C++中打印错误的方式
//             exit(FORK_CREATE_ERROR);
//         }
//         else if (id == 0) // 子进程
//         {
//             close(pipefd[1]); // 子进程读，关闭写端
//             Task(pipefd[0]);  // 从管道中读取任务
//             exit(NO_PROBLEM);
//         }
//         else
//         {
//             // 父进程执行的任务
//             // 子进程不会执行后续的代码，在执行完自己的任务后就退出了，执行循环任务的只有父进程
//             close(pipefd[0]); // 父进程写，关闭读端

//             // // 保存管道的写端,将管道写文件描述符和子进程的 pid 传给 channel 类对象
//             // Channel ch(pipefd[1], id);
//             // // 保存到组织 Channel 类的容器中
//             // channels.push_back(ch);
//             channels.emplace_back(pipefd[1], id);

//             // 测试
//             std::cout << "创建子进程成功: " << id << std::endl;
//             sleep(2);
//         }
//     }
// }

// 创建进程池类
class ProcessPool
{
private:
    // 定义管道类
    class Channel
    {
    public:
        Channel(int wfd, pid_t pid) // 构造函数
            : _wfd(wfd), _child_pid(pid)
        {
            // 自定义实现子进程的名字，to_string 函数将整型转化成字符串
            _child_name = "child_name-" + std::to_string(_child_pid);
        }
        ~Channel() {} // 析构函数

        void PrintInfo()
        {
            printf("wfd: %d, child_pid: %d, child_name: %s\n", _wfd, _child_pid, _child_name.c_str());
        }

        void WriteTask(int task)
        {
            ssize_t n = write(_wfd, &task, sizeof(task)); // 这就是约定的4字节发送
            (void)n;
        }

        // 获取 Channel 名字
        std::string GetName() { return _child_name; }

        // 关闭管道
        void ClosePipe()
        {
            std::cout << "关闭文件描述符：" << _wfd << std::endl;
            close(_wfd);
        }

        // 等待子进程
        void WaitProcess()
        {
            pid_t rid = waitpid(_child_pid, nullptr, 0);
            (void)rid;
        }

    private:
        // 成员属性
        int _wfd;                // 写文件描述符
        pid_t _child_pid;        // 对应的子进程的pid
        std::string _child_name; // 管道的名称
    };

public:
    ProcessPool() {}  // 构造函数
    ~ProcessPool() {} // 析构函数

    // // 初始化进程池
    // void InitProcessPool(task_t task)
    // {
    //     CreatProcessAndChannel(task);
    // }

    // 初始化进程池
    void InitProcessPool(cb_t cb)
    {
        CreatProcessAndChannel(cb);
    }

    // 打印子进程的信息
    void PrintProcess()
    {
        for (auto &ch : channels)
        {
            ch.PrintInfo();
        }
    }

    // 进程池运行
    void RunProcessPool()
    {
        int cnt = 10;
        while (cnt--)
        // while(1)
        {
            std::cout << "-----------------------------" << std::endl;
            // 1. 选择一个 channel（管道+子进程）,本质上是在 vector 中选择一个下标数字
            int whoindex = SelectChannel();

            // 测试是否是轮询式的选择子进程
            std::cout << "selected index: " << whoindex << std::endl;
            // sleep(1);

            // 2. 随机选择一个任务
            int task = SelectTask();

            // 测试是否选择到了一个任务
            std::cout << "taskindex: " << task << std::endl;
            // sleep(1);

            // 3. 发送一个任务给指定的 channel(管道+子进程)
            SendTasktoSalver(task, whoindex);

            // 测试
            printf("发送任务下标为 %d to %s\n", task, channels[whoindex].GetName().c_str());
            sleep(1);
        }
    }

    int SelectChannel() // 选择一个进程
    {
        // 使用轮询的方法选择子进程
        static int index = 0; // 默认初始时选择下标为0的子进程
        int select = index;
        index++;                  // 轮询选择
        index %= channels.size(); // 避免下标越界

        return select;
    }

    int SelectTask() // 选择一个任务
    {
        srand((unsigned int)time(NULL));       // 定义随机数种子
        int taskindex = rand() % tasks.size(); // 在任务列表中随机选择任务下标

        return taskindex;
    }

    void SendTasktoSalver(int task, int whoindex)
    {
        // 如何将任务发送给进程？
        // 管道都是由channels管理的，将对应的任务码task写给给指定的进程(whoindex)即可
        // WriteTask 函数实现在 Channel 类内部
        channels[whoindex].WriteTask(task);
    }

    // 进程池退出
    void QuitProcessPool()
    {
        // 只需要父进程关闭管道的写端，子进程读取管道的结尾时，会自动退出
        // 退出后子进程会进入僵尸状态，需要父进程回收子进程

        // // version1:
        // // 1. 退出所有的子进程
        // for(auto& channel : channels)   // 关闭所有的管道
        // {
        //     channel.ClosePipe();
        // }

        // // 2. 回收子进程
        // for(auto& channel : channels)   // 等待子进程
        // {
        //     channel.WaitProcess();
        // }

        // bug演示:
        // for(auto& channel : channels)
        // {
        //     channel.ClosePipe();    // 1. 退出所有的子进程
        //     channel.WaitProcess();  // 2. 回收子进程
        // }

        // version2: 逆向回收
        // int end = channels.size() - 1;
        // while(end >= 0)
        // {
        //     channels[end].ClosePipe();
        //     channels[end].WaitProcess();
        //     end--;
        // }

        // version3: 子进程关闭父进程的历史写文件描述符
        for (auto &channel : channels)
        {
            channel.ClosePipe();   // 1. 退出所有的子进程
            channel.WaitProcess(); // 2. 回收子进程
        }
    }

private:
    // 1. 创建多个进程和管道
    void CreatProcessAndChannel(cb_t cb)
    {
        for (int i = 0; i < processNum; i++)
        {
            // 1. 创建多个管道
            int pipefd[2] = {0};  // 文件描述符数组
            int n = pipe(pipefd); // 创建管道
            if (n < 0)            // 创建管道失败
            {
                std::cerr << "pipe create error" << std::endl; // 使用C++中打印错误的方式
                exit(PIPE_CREATE_ERROR);
            }

            // 创建管道成功，创建子进程
            pid_t id = fork();
            if (id < 0) // 创建子进程失败
            {
                std::cerr << "chile process create error" << std::endl; // 使用C++中打印错误的方式
                exit(FORK_CREATE_ERROR);
            }
            else if (id == 0) // 子进程
            {
                // version3:
                // 关闭父进程的历史上的写文件描述符
                if (!channels.empty()) // channels不为空，需要操作
                {
                    for (auto &channel : channels)
                    {
                        channel.ClosePipe();
                    }
                }

                close(pipefd[1]); // 子进程读，关闭写端
                // Task(pipefd[0]);  // 从管道中读取任务
                cb(pipefd[0]); // 从管道中读取任务
                exit(NO_PROBLEM);
            }
            else
            {
                // 父进程执行的任务
                // 子进程不会执行后续的代码，在执行完自己的任务后就退出了，执行循环任务的只有父进程
                close(pipefd[0]); // 父进程写，关闭读端

                // // 保存管道的写端,将管道写文件描述符和子进程的 pid 传给 channel 类对象
                // Channel ch(pipefd[1], id);
                // // 保存到组织 Channel 类的容器中
                // channels.push_back(ch);
                channels.emplace_back(pipefd[1], id);

                // 测试
                std::cout << "创建子进程成功: " << id << std::endl;
                sleep(2);
            }
        }
    }

private:
    // 组织所有的 Channel
    std::vector<Channel> channels; // 成员变量
};

int main()
{
    // // 组织所有的 Channel
    // std::vector<Channel> channels;

    ProcessPool pp; // 创建进程池对象
    // pp.InitProcessPool(); // 初始化进程池
    pp.InitProcessPool(Task); // 初始化进程池
    pp.PrintProcess();

    // 2. 父进程控制子进程
    // pp.PrintProcess();
    pp.RunProcessPool(); // 运行进程池
    std::cout << std::endl;

    // 3. 释放和回收管道和子进程
    pp.QuitProcessPool();

    return 0;
}

// // 1. 创建多个管道
// int pipefd[2] = {0};  // 文件描述符数组
// int n = pipe(pipefd); // 创建管道
// if (n < 0)            // 创建管道失败
// {
//     std::cerr << "pipe create error" << std::endl; // 使用C++中打印错误的方式
//     return PIPE_CREATE_ERROR;
// }

// // 创建管道成功，创建子进程
// pid_t id = fork();
// if (id < 0) // 创建子进程失败
// {
//     std::cerr << "chile process create error" << std::endl; // 使用C++中打印错误的方式
//     return FORK_CREATE_ERROR;
// }
// else if (id == 0) // 子进程
// {
//     close(pipefd[1]); // 子进程读，关闭写端
//     Task();
//     return NO_PROBLEM;
// }
// else // 父进程
// {
// }

// // 父进程执行的任务
// close(pipefd[0]); // 父进程写，关闭读端

