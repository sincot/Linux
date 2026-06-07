#include <iostream>
#include <signal.h>

// void PrintPending(const sigset_t &pending_set, int cnt)
void PrintPending(const sigset_t &pending_set)
{
    // 判断所有普通信号是否在 pending 信号集中
    for (int signo = 31; signo > 0; signo--) // 从高位开始打印
    {
        // 判断普通信号是否在 pending 信号集中,使用 sigismember
        // int sigismember(const sigset_t *set, int signum)
        if (sigismember(&pending_set, signo)) // 为真即为1
        {
            std::cout << "1 ";
        }
        else
        {
            std::cout << "0 ";
        }
    }

    std::cout << std::endl;
    // std::cout << "  cnt: " << cnt << std::endl;
    // std::cout << "判断完毕" << std::endl;
}

// int main()
// {
//     sigset_t block_set, old_set;    // 设置信号集

//     // 调用 sigemptyset 系统调用，将 set 信号集全置为0
//     // int sigemptyset(sigset_t *set);
//     sigemptyset(&block_set);
//     sigemptyset(&old_set);

//     // 将 2 号信号添加到 block_set 信号集中
//     // int sigaddset(sigset_t *set, int signum);
//     sigaddset(&block_set, SIGINT);

//     // 0. 屏蔽 2 号信号 —— 使用 sigprocmask 函数
//     // int sigprocmask(int how, const sigset_t *_Nullable restrict set，sigset_t *_Nullable restrict oldset)
//     // how —— SIG_SETMASK：设置当前信号屏蔽字为set所指向的值
//     sigprocmask(SIG_SETMASK, &block_set, &old_set);

//     std::cout << "process pid: " << getpid() << std::endl;

//     int cnt = 0;
//     while(true)     // 循环获取并打印 pending 集
//     {
//         sigset_t pending_set;   // 设置 pending 信号集
//         sigemptyset(&block_set);    // 初始化 pending 信号集

//         // 1. 获取 pending 信号集
//         // int sigpending(sigset_t *set)
//         sigpending(&pending_set);

//         // 2. 打印 pending 信号集
//         PrintPending(pending_set, cnt);

//         // 增加一个计数器，当技术器满足某种条件时，解除对2号信号的屏蔽
//         if(cnt == 6)
//         {
//             // 3. 解除对 2 号信号的屏蔽
//             std::cout << "解除对2号信号的屏蔽" << std::endl;
//             // 调用 sigprocmask,将老的屏蔽字写回去
//             sigprocmask(SIG_SETMASK, &old_set, nullptr);
//         }
//         cnt++;

//         sleep(3);
//     }

//     return 0;
// }

// void handler(int signo)
// {
//     // 更改 2 号信号的默认终止行为
//     std::cout << "处理完毕：" << signo << std::endl;
// }

// // 捕捉2号信号
// int main()
// {
//     signal(2, handler);

//     sigset_t block_set, old_set;    // 设置信号集

//     // 调用 sigemptyset 系统调用，将 set 信号集全置为0
//     // int sigemptyset(sigset_t *set);
//     sigemptyset(&block_set);
//     sigemptyset(&old_set);

//     // 将 2 号信号添加到 block_set 信号集中
//     // int sigaddset(sigset_t *set, int signum);
//     sigaddset(&block_set, SIGINT);

//     // 0. 屏蔽 2 号信号 —— 使用 sigprocmask 函数
//     // int sigprocmask(int how, const sigset_t *_Nullable restrict set，sigset_t *_Nullable restrict oldset)
//     // how —— SIG_SETMASK：设置当前信号屏蔽字为set所指向的值
//     sigprocmask(SIG_SETMASK, &block_set, &old_set);

//     std::cout << "process pid: " << getpid() << std::endl;

//     int cnt = 0;
//     while(true)     // 循环获取并打印 pending 集
//     {
//         sigset_t pending_set;   // 设置 pending 信号集
//         sigemptyset(&block_set);    // 初始化 pending 信号集

//         // 1. 获取 pending 信号集
//         // int sigpending(sigset_t *set)
//         sigpending(&pending_set);

//         // 2. 打印 pending 信号集
//         PrintPending(pending_set, cnt);

//         // 增加一个计数器，当技术器满足某种条件时，解除对2号信号的屏蔽
//         if(cnt == 6)
//         {
//             // 3. 解除对 2 号信号的屏蔽
//             std::cout << "解除对2号信号的屏蔽" << std::endl;
//             // 调用 sigprocmask,将老的屏蔽字写回去
//             sigprocmask(SIG_SETMASK, &old_set, nullptr);
//         }
//         cnt++;

//         sleep(3);
//     }

//     return 0;
// }

void handler(int signo)
{
    std::cout << "##### handler enter ########" << std::endl;
    // 更改 2 号信号的默认终止行为
    std::cout << "处理完毕：" << signo << std::endl;
    sigset_t handler_set;
    sigemptyset(&handler_set); // 初始化 pending 信号集
    sigpending(&handler_set);

    // 2. 打印 pending 信号集
    PrintPending(handler_set);
    std::cout << "##### handler enter ########" << std::endl;
}

// 捕捉2号信号
int main()
{
    signal(2, handler);

    sigset_t block_set, old_set; // 设置信号集

    // 调用 sigemptyset 系统调用，将 set 信号集全置为0
    // int sigemptyset(sigset_t *set);
    sigemptyset(&block_set);
    sigemptyset(&old_set);

    // 将 2 号信号添加到 block_set 信号集中
    // int sigaddset(sigset_t *set, int signum);
    sigaddset(&block_set, SIGINT);

    // 0. 屏蔽 2 号信号 —— 使用 sigprocmask 函数
    // int sigprocmask(int how, const sigset_t *_Nullable restrict set，sigset_t *_Nullable restrict oldset)
    // how —— SIG_SETMASK：设置当前信号屏蔽字为set所指向的值
    sigprocmask(SIG_SETMASK, &block_set, &old_set);

    std::cout << "process pid: " << getpid() << std::endl;

    int cnt = 0;
    while (true) // 循环获取并打印 pending 集
    {
        sigset_t pending_set;    // 设置 pending 信号集
        sigemptyset(&block_set); // 初始化 pending 信号集

        // 1. 获取 pending 信号集
        // int sigpending(sigset_t *set)
        sigpending(&pending_set);

        // 2. 打印 pending 信号集
        PrintPending(pending_set);

        // 增加一个计数器，当技术器满足某种条件时，解除对2号信号的屏蔽
        if (cnt == 6)
        {
            // 3. 解除对 2 号信号的屏蔽
            std::cout << "解除对2号信号的屏蔽" << std::endl;
            // 调用 sigprocmask,将老的屏蔽字写回去
            sigprocmask(SIG_SETMASK, &old_set, nullptr);
        }
        cnt++;

        sleep(3);
    }

    return 0;
}

// 屏蔽所有信号
// int main()
// {
//     sigset_t block_set, old_set; // 设置信号集

//     // 调用 sigemptyset 系统调用，将 set 信号集全置为0
//     // int sigemptyset(sigset_t *set);
//     sigemptyset(&block_set);
//     sigemptyset(&old_set);

//     for (int signo = 1; signo < 32; signo++)
//     {
//         // 将所有信号添加到 block_set 信号集中
//         // int sigaddset(sigset_t *set, int signum);
//         sigaddset(&block_set, signo);
//     }

//     // 0. 屏蔽所有信号 —— 使用 sigprocmask 函数
//     // int sigprocmask(int how, const sigset_t *_Nullable restrict set，sigset_t *_Nullable restrict oldset)
//     // how —— SIG_SETMASK：设置当前信号屏蔽字为set所指向的值
//     sigprocmask(SIG_SETMASK, &block_set, &old_set);

//     while (true) // 循环获取并打印 pending 集
//     {
//         sigset_t pending_set;    // 设置 pending 信号集
//         sigemptyset(&block_set); // 初始化 pending 信号集

//         // 1. 获取 pending 信号集
//         // int sigpending(sigset_t *set)
//         sigpending(&pending_set);

//         // 2. 打印 pending 信号集
//         PrintPending(pending_set);

//         sleep(3);
//     }

//     return 0;
// }

// 只屏蔽 2 号信号
// int main()
// {
//     sigset_t block_set, old_set;    // 设置信号集

//     // 调用 sigemptyset 系统调用，将 set 信号集全置为0
//     // int sigemptyset(sigset_t *set);
//     sigemptyset(&block_set);
//     sigemptyset(&old_set);

//     // 将 2 号信号添加到 block_set 信号集中
//     // int sigaddset(sigset_t *set, int signum);
//     sigaddset(&block_set, SIGINT);

//     // 0. 屏蔽 2 号信号 —— 使用 sigprocmask 函数
//     // int sigprocmask(int how, const sigset_t *_Nullable restrict set，sigset_t *_Nullable restrict oldset)
//     // how —— SIG_SETMASK：设置当前信号屏蔽字为set所指向的值
//     sigprocmask(SIG_SETMASK, &block_set, &old_set);

//     while(true)     // 循环获取并打印 pending 集
//     {
//         sigset_t pending_set;   // 设置 pending 信号集
//         sigemptyset(&block_set);    // 初始化 pending 信号集

//         // 1. 获取 pending 信号集
//         // int sigpending(sigset_t *set)
//         sigpending(&pending_set);

//         // 2. 打印 pending 信号集
//         PrintPending(pending_set);

//         sleep(3);
//     }

//     return 0;
// }
