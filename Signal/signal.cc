#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>


void handler(int signo)
{
    std::cout << "receive a signal: " << signo << " pid: " << getpid() << std::endl;
    // exit(10);
}

int main()
{
    while(true)
    {
        std::cout << "I am process: " << getpid() << std::endl;
        sleep(3);
    }
    
    // pid_t id = fork();
    // if(id == 0)
    // {
    //     std::cout << "child process: " << getpid() << std::endl;
    //     sleep(3);

    //     // 模拟除0错误
    //     int a = 10;
    //     a /= 0;
    //     exit(10);
    // }
    // // 父进程等待子进程
    // int status = 0;
    // int wid = waitpid(id, &status, 0);
    // printf("exit code: %d, exit signal: %d, core dumped: %d\n", (status>>8)&0xFF, status&0x7F, (status>>7)&0x1);


    // sighandler_t signal(int signum, sighandler_t handler);
    // for(int signumber = 1; signumber <= 31; signumber++)
    // {
    //     signal(signumber, handler);    // 将所有普通信号都自定义
    // }

    // signal(SIGFPE, handler);
    // signal(SIGSEGV, handler);
    
    // while(1)
    // {
    //     std::cout << "main main " << getpid() << std::endl;
    //     sleep(5);

    //     // 模拟除0
    //     int a = 10;
    //     a /= 0;

    //     // 野指针
    //     // int *p = nullptr;
    //     // *p = 10;
    // }

    return 0;
}

