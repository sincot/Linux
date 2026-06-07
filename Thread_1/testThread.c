#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
// #include <iostream>

// 演示使用多线程
// void *threadRun(void *args)
// {
//     while (1)
//     {
//         // std::cout << "new thread" << std::endl;
//         printf("new thread, pid: %d\n", getpid());
//         sleep(1);
//     }
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, NULL, threadRun, NULL);
//     // pthread_create(&tid, nullptr, threadRun, nullptr);

//     while (1)
//     {
//         // std::cout << "main thread" << std::endl;
//         printf("main thread, pid: %d\n", getpid());
//         sleep(1);
//     }

//     return 0;
// }


