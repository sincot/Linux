#include <iostream>
#include <signal.h>

// 规范用户输入指令的格式
void User(const std::string &cmd)
{
    std::cout << "User: " << cmd << "signumber who" << std::endl; 
}

long long cnt = 1;

void handler(int signo)
{
    std::cout << "获取到信号: " << signo << " pid " << getpid() << std::endl;
    int n = alarm(2);
    std::cout << "上一个闹钟剩余的时间: " << n << std::endl; 
    // abort();
}

int main(int argc, char *argv[])
{
    // signal(2, handler);
    // signal(6, handler);
    // alarm(1);   // 设置 1 秒后的闹钟
    // alarm(2);   // 设置 2 秒后的闹钟
    // signal(SIGALRM, handler);   // 捕捉闹钟
    // alarm(200);   // 设置 20 秒后的闹钟
    
    alarm(200);
    sleep(1);
    int n = alarm(0);
    std::cout << "上一个闹钟剩余的时间: " << n << std::endl; 
    
    while(true)
    {
        // int n = alarm(3);
        // std::cout << "进程正在运行: " << cnt << " n: " << n << std::endl;
        std::cout << "进程正在运行: " << cnt << std::endl;

        sleep(1);

        // std::cout << "死循环 " << getpid() << std::endl;
        // std::cout << "进程正在运行: " << cnt << std::endl;
        // cnt++;
        // sleep(3);
        // abort();
        // raise(2);   // 自己向自己发送 2 号信号
    }


    // // 输入的格式必须是：./mykill signumber pid
    // if(argc != 3)
    // {
    //     // argv 命令行参数：./mykill signumber pid
    //     User(argv[0]);
    //     exit(1);
    // }

    // // argv[1]的内容就是输入的 signumber，将其转化为整型
    // int signumber = std::stoi(argv[1]);
    // // argv[2]的内容就是进程的 pid，将其转化为整型
    // pid_t id = std::stoi(argv[2]);

    // // 调用 kill 系统调用，向进程发送信号
    // int n = kill(id, signumber);

    return 0;
}

