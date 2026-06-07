#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int main()
{
    // 1. 创建管道
    // 定义一个数组，作为 pipe 函数的参数
    int pipefd[2] = { 0 };
    // 创建管道
    int n = pipe(pipefd);

    // 创建失败返回-1
    if(n < 0)
    {
        perror("pipe");
        return 1;   // 返回错误码信息
    }
    
    // 2. fork创建子进程
    pid_t id = fork();
    // 3. 让子进程写，父进程读
    if(id == 0) // 子进程
    {
        // child 关闭读端，保留写端
        close(pipefd[0]);

        // // 开始写数据
        // char* msg = "hello world";
        
        // 持续写入数据
        char* msg = "hello parent";
        int cnt = 10;
        // 要让父进程接收到变化的字符串
        char outbuffer[512];
        while(cnt)
        {
            // 格式化输出：int snprintf(char str[restrict .size], size_t size, const char *restrict format, ...)
            // 将输出的格式化信息写入到 str 数组中
            // 这样做可以让写入父进程的数据是变化的，更能直观的感受到通信
            snprintf(outbuffer, sizeof(outbuffer), "c->f# %s %d %d\n", msg, cnt--, getpid());
            // 在使用 snprintf 函数时，如果格式化字符串太长了，岂不是就没有\0的位置了？
            // 当 strlen 求取字符串长度时，不就会把乱码计算在内吗？
            // snprintf 是C语言函数，outbuffer的大小是512，snprintf使用outbuffer时，只会使用511大小，最后一个字符永远自动添加\0

            // write: ssize_t write(int fd, const void buf[.count], size_t count)
            // 向写端写入msg信息，大小为strlen(msg)
            write(pipefd[1], outbuffer, strlen(outbuffer));
            srand((unsigned int)time(NULL));
            if(rand() % cnt == 0)
                sleep(1);   // 每隔1秒写入一次
        }
    }
    
    // 父进程
    // parent 关闭写段，保留读端
    close(pipefd[1]);
    // 创建文件缓冲区
    char inbuffer[512];
    // 持续的读
    while(1)
    {
        // read: ssize_t read(int fd, void buf[.count], size_t count)
        ssize_t n = read(pipefd[0], inbuffer, sizeof(inbuffer) - 1);
        // 为什么要-1？ 保证即便 inbuffer 读满了，仍然保留一个位置放置 \0
        
        // 返回值大于0，在管道中读到了有效的数据
        if(n > 0)
        {
            inbuffer[n] = 0;    // 在读取到的字符串末尾加上\0
            printf("%s\n", inbuffer);     // 打印读到的字符串
        }
        else if(n == 0)
        {}
        else
        {
            perror("read");
            break;
        }
    }

    // 父进程等待子进程
    pid_t rid = waitpid(id, NULL, 0);
    (void)rid; // 不使用rid返回值，防止编译器告警
    
    return 0;
}

// 第一步：创建管道
// // 定义一个数组，作为 pipe 函数的参数
//     int pipefd[2] = { 0 };
//     // 创建管道
//     int n = pipe(pipefd);

//     // 创建失败返回-1
//     if(n < 0)
//     {
//         perror("pipe");
//         return 1;   // 返回错误码信息
//     }
//     // 查看pipefd数组中的内容
//     // 显然 pipefd[0]: 3; pipefd[1]: 4
//     printf("pipefd[0]: %d, pipefd[1]: %d\n", pipefd[0], pipefd[1]);

// 第二步：fork创建子进程
// // 2. fork创建子进程
//     pid_t id = fork();
//     if(id == 0) // 子进程
//     {
        
//     }
    
//     // 父进程
//     // 父进程等待子进程
//     pid_t rid = waitpid(id, NULL, 0);
//     (void)rid; // 不使用rid返回值，防止编译器告警

// // 开始写数据
//         char* msg = "hello world";
//         // write: ssize_t write(int fd, const void buf[.count], size_t count)
//         // 向写端写入msg信息，大小为strlen(msg)
//         write(pipefd[1], msg, strlen(msg));
