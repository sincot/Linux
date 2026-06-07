#pragma once

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

// 是否需要切换工作目录，是否需要重定向标准输入、输出和错误，为1表示需要，为0表示不需要
void Daemon(int dir, int dup)     
{
    // 1. 忽略某些信号
    // 守护进程需要忽略 SIGCHLD 信号，以防止子进程成为僵尸进程
    signal(SIGCHLD, SIG_IGN);
    // 守护进程需要忽略 SIGPIPE 信号，以防止管道破裂时进程被终止
    signal(SIGPIPE, SIG_IGN);

    // 2. 进程不能是组长进程
    if (fork() > 0)
    {
        // 创建子进程成功，父进程退出
        // 后续的任务由子进程来完成
        exit(0);
    }

    // 子进程执行后续的任务
    // 3. 创建新的会话，脱离控制终端 —— 调用 setsid() 函数
    // 原型：pid_t setsid(void);
    setsid();

    // 4. 将当前工作目录切换到根目录，避免占用某个目录 —— 可以不切换
    // 原型：int chdir(const char *path);
    // 功能：将当前工作目录切换到指定的路径
    if(dir == 1) chdir("/");

    // 5. 重定向 0，1，2 三个文件描述符到 /dev/null 文件中 —— 可以不重定向
    if(dup == 1)
    {
        int fd = open("/dev/null", O_RDWR); // 以读写的方式打开 /dev/null 文件，获取文件描述符
        if (fd >= 0)
        {
            // dup2 函数的作用是将一个文件描述符复制到另一个文件描述符上 —— 重定向
            // 原型：int dup2(int oldfd, int newfd);
            dup2(fd, 0); // 将标准输入重定向到 /dev/null
            dup2(fd, 1); // 将标准输出重定向到 /dev/null
            dup2(fd, 2); // 将标准错误重定向到 /dev/null
            close(fd);   // 关闭原始的文件描述符
        }
    }
    else 
    {
        // 不重定向，直接关闭标准输入、输出和错误
        close(0);
        close(1);
        close(2);
    }
}