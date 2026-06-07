#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cstdio>
#include <stdlib.h>
#include <sys/wait.h>

// 测试 sigaction 函数的使用
// void handler(int signo)
// {
//     std::cout << "捕捉一个信号：" << signo << std::endl;
// }

// int main()
// {
//     struct sigaction act, oldact;
//     act.sa_handler = handler;

//     // 调用 sigaction 函数
//     // 函数原型: int sigaction(int signum,
//     //              const struct sigaction *_Nullable restrict act,
//     //              struct sigaction *_Nullable restrict oldact);
//     sigaction(SIGINT, &act, &oldact);

//     while(true)
//     {
//         std::cout << "process is running —— " << getpid() << std::endl;
//         sleep(1);
//     }
// }

// 证明：
// void handler(int signo)
// {
//     std::cout << "捕捉一个信号：" << signo << std::endl;

//     sigset_t pending;
//     while(true)
//     {
//         sigpending(&pending);
//         for(int signo = 31; signo > 0; signo--)
//         {
//             // 判断该信号是否存在 pending 信号集中
//             if(sigismember(&pending, signo))
//             {
//                 std::cout << "1 ";
//             }
//             else
//             {
//                 std::cout << "0 ";
//             }
//         }
//         sleep(1);
//         std::cout << std::endl;
//     }
// }

// int main()
// {
//     struct sigaction act, oldact;
//     act.sa_handler = handler;
//     // 将 struct sigaction 中的成员变量 sa_mask 中的内容清零
//     sigemptyset(&(act.sa_mask));
//     // sigaddset(&(act.sa_mask), signo);    // 添加其它的信号
//     act.sa_flags = 0;
//     act.sa_restorer = nullptr;

//     // 调用 sigaction 函数
//     // 函数原型: int sigaction(int signum,
//     //              const struct sigaction *_Nullable restrict act,
//     //              struct sigaction *_Nullable restrict oldact);
//     sigaction(SIGINT, &act, &oldact);

//     while(true)
//     {
//         std::cout << "process is running —— " << getpid() << std::endl;
//         sleep(1);
//     }
// }

// volatile 关键字的介绍
// volatile int flag = 0;

// void handler(int signo)
// {
//     flag = 1;
//     printf("flag 由 0 -> 1\n");
// }

// int main()
// {
//     signal(2, handler);

//     while(!flag);
//     printf("进程正在退出\n");

//     return 0;
// }

// SIGCHLD 信号的介绍
// void handler(int signo)
// {
//     printf("获取到了信号, 信号编号为: %d, pid: %d\n", signo, getpid());
// }

// int main()
// {
//     signal(SIGCHLD, handler);

//     pid_t id = fork();
//     if(id == 0)
//     {
//         printf("child process exit\n");
//         sleep(5);
//         exit(10);
//     }

//     while(true)
//     {
//         printf("father process running: %d\n", getpid());
//         sleep(1);
//     }

//     return 0;
// }

// void handler(int signo)
// {
//     printf("获取到了信号, 信号编号为: %d, pid: %d\n", signo, getpid());
//     int status = 0;
//     waitpid(-1, &status, 0);
//     printf("staus code: %d\n", WEXITSTATUS(status));
// }

// int main()
// {
//     signal(SIGCHLD, handler);

//     pid_t id = fork();
//     if(id == 0)
//     {
//         printf("child process exit, pid: %d\n", getpid());
//         sleep(5);
//         exit(10);
//     }

//     while(true)
//     {
//         printf("father process running: %d\n", getpid());
//         sleep(1);
//     }

//     return 0;
// }

// 问题1：10个子进程，同时退出
// void handler(int signo)
// {
//     printf("获取到了信号, 信号编号为: %d, pid: %d\n", signo, getpid());
//     int status = 0;
//     while (1)
//     {
//         pid_t wid = waitpid(-1, &status, 0);
//         if (wid < 0)
//         {
//             break;
//         }
//     }
//     printf("wait done\n");
// }

// int main()
// {
//     // 一旦任意一个子进程退出了，就会捕捉到 SIGCHLD 信号 
//     signal(SIGCHLD, handler); // 父进程执行的

//     // 创建 10 个子进程
//     for (int process_index = 0; process_index < 10; process_index++)
//     {
//         pid_t id = fork();
//         if (id == 0)
//         {
//             printf("child process exit, pid: %d\n", getpid());
//             sleep(5);
//             exit(10);
//         }
//     }

//     while (true)
//     {
//         printf("father process running: %d\n", getpid());
//         sleep(1);
//     }

//     return 0;
// }

// 问题2：10个子进程，9个子进程退出，1个子进程不退出
// void handler(int signo)
// {
//     printf("获取到了信号, 信号编号为: %d, pid: %d\n", signo, getpid());
//     int status = 0;
//     while (1)
//     {
//         pid_t wid = waitpid(-1, &status, 0);
//         if (wid < 0)
//         {
//             break;
//         }
//     }
//     printf("wait done\n");
// }

// int main()
// {
//     // 一旦任意一个子进程退出了，就会捕捉到 SIGCHLD 信号 
//     signal(SIGCHLD, handler); // 父进程执行的

//     // 创建 10 个子进程
//     for (int process_index = 0; process_index < 10; process_index++)
//     {
//         pid_t id = fork();
//         if (id == 0)
//         {
//             if(process_index == 4)  // 设置某个进程不退出
//             {
//                 printf("%d 进程不退出, pid: %d\n", process_index, getpid());
//                 sleep(1000);    // 该子进程休眠1000秒并不影响气其它进程
//             }
//             printf("child process exit, pid: %d\n", getpid());
//             sleep(5);
//             exit(10);
//         }
//     }

//     while (true)
//     {
//         printf("father process running: %d\n", getpid());
//         sleep(1);
//     }

//     return 0;
// }

// 解决问题二: 1
// void handler(int signo)
// {
//     printf("获取到了信号, 信号编号为: %d, pid: %d\n", signo, getpid());
//     int status = 0;
//     while (1)
//     {
//         pid_t wid = waitpid(-1, &status, WNOHANG);  // 非阻塞等待
//         // 非阻塞等待，返回值会有三种
//         // wid > 0: waitpid 调用成功
//         // wid < 0: 发生错误
//         // wid == 0: waitpid 调用成功，但是没有子进程退出
//         if (wid <= 0)
//         {
//             break;
//         }
//     }
//     printf("wait done\n");
// }

// int main()
// {
//     // 一旦任意一个子进程退出了，就会捕捉到 SIGCHLD 信号 
//     signal(SIGCHLD, handler); // 父进程执行的

//     // 创建 10 个子进程
//     for (int process_index = 0; process_index < 10; process_index++)
//     {
//         pid_t id = fork();
//         if (id == 0)
//         {
//             if(process_index == 4)  // 设置某个进程不退出
//             {
//                 printf("%d 进程不退出, pid: %d\n", process_index, getpid());
//                 sleep(1000);    // 该子进程休眠1000秒并不影响气其它进程
//             }
//             printf("child process exit, pid: %d\n", getpid());
//             sleep(5);
//             exit(10);
//         }
//     }

//     while (true)
//     {
//         printf("father process running: %d\n", getpid());
//         sleep(1);
//     }

//     return 0;
// }

// 其它不出现僵尸进程的方法
int main()
{
    // 一旦任意一个子进程退出了，就会捕捉到 SIGCHLD 信号 
    // 将 SIGCHLD 的处理动作置为 SIG_IGN,用户明确忽略
    signal(SIGCHLD, SIG_IGN); // 父进程执行的

    // 创建 10 个子进程
    for (int process_index = 0; process_index < 10; process_index++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            if(process_index == 4)  // 设置某个进程不退出
            {
                printf("%d 进程不退出, pid: %d\n", process_index, getpid());
                sleep(1000);    // 该子进程休眠1000秒并不影响气其它进程
            }
            printf("child process exit, pid: %d\n", getpid());
            sleep(5);
            exit(10);
        }
    }

    while (true)
    {
        printf("father process running: %d\n", getpid());
        sleep(1);
    }

    return 0;
}