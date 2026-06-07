#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <time.h>


// 进程随机调度
int current = 0;
int counter = 5;

struct task_struct
{
public:
    // 时间片初始值为5
    task_struct(int pid)
        : _pid(pid), _counter(counter) 
    {}

    void desc_timer()   // 时间片的减少
    {
        _counter--;
    }

    bool Expired()      // 判断时间片是否过期
    {
        return _counter <= 0;
    }

    void ResetCounter() // 重置进程的时间片
    {
        _counter = counter;
    }

    int ProcessPid()   // 获取当前进程的 pid
    {
        return _pid;
    }

    ~task_struct() {}

    void ProcessRun()
    {
        std::cout << "process " << _pid << " running" << std::endl;
    }

private:
    int _pid;        // 标识码
    int _status;     // 状态码
    int _counter;    // 时间片
};

// 进程的管理
std::vector<task_struct> tasks;

// 处理时钟中断
void do_timer(int signo)    
{
    // 触发时钟中断，让进程的时间片减少1
    tasks[current].desc_timer();

    if(tasks[current].Expired())        // 检测时间片是否过期
    {
        std::cout << tasks[current].ProcessPid() << " 已过期，重新选择进程调度" << std::endl;
        // 重置当前进程的时间片
        tasks[current].ResetCounter();

        // 时间片到了，随机选择一个进程运行
        current = rand() % tasks.size();
    }
    else
    {
        // 时间片没过期，进程运行
        tasks[current].ProcessRun();
    }

    alarm(1);   // 重新设置闹钟
}

int main()
{
    alarm(1);   // 1 s钟触发闹钟
    signal(SIGALRM, do_timer);   // 捕捉 SIGALRM 信号

    srand((unsigned int)time(nullptr));     // 种下随机数种子

    // 向进程管理表中添加进程
    tasks.emplace_back(1);
    tasks.emplace_back(2);
    tasks.emplace_back(3);
    tasks.emplace_back(4);
    tasks.emplace_back(5);
    tasks.emplace_back(6);

    for(;;)
    {
        // std::cout << "OS 被中断唤醒" << std::endl; 
        pause();    // 暂停
    }
       
    return 0;
}

