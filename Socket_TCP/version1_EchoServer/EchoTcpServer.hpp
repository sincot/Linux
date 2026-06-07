#pragma once

#include "Logger.hpp"
#include "InetAddr.hpp"
#include "ThreadPool.hpp"

#include <iostream>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/wait.h>
// #include <signal.h>
#include <pthread.h>
#include <functional>


using namespace LOGMOUDLE;
using namespace THREAD_POOL;

const int gbacklog = 32;
const uint16_t gport = 8080;

using task_t = std::function<void()>;   // 返回值为void，参数为空


enum
{
    SUCCESS,
    USAGE_ERR,
    SOCKET_ERR,
    BIND_ERR,
    LISTEN_ERR,
    FORK_ERR,
};

class TcpServer
{
public:
    TcpServer(uint16_t port = gport) : _port(port)
    {
    }

    ~TcpServer()
    {
        close(_listensockfd);
    }

    void InitTcpServer()
    {
        // 1. 创建 socket —— int socket(int domain, int type, int protocol)
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM 流式套接字
        if (_listensockfd < 0)                           // 创建 socket 失败
        {
            LOG(LogLevel::FATAL) << "create socket failed";
            exit(SOCKET_ERR);
        }
        LOG(LogLevel::DEBUG) << "create socket success: " << _listensockfd;

        // 2. 填充本地 socket 信息
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);      // h -> ns
        local.sin_addr.s_addr = INADDR_ANY; // 任意地址绑定

        // 3. 绑定 —— int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
        int n = bind(_listensockfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0) // 绑定失败
        {
            LOG(LogLevel::FATAL) << "bind failed";
            exit(BIND_ERR);
        }
        LOG(LogLevel::DEBUG) << "bind socket success";

        // 4. tcp 是面向连接的，在通信之前，客户端需要向服务器发送请求
        // 因此，TCP 服务器需要处于一种 listen 监听状态
        // int listen(int sockfd, int backlog);
        n = listen(_listensockfd, gbacklog);
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "listen failed";
            exit(LISTEN_ERR);
        }
        LOG(LogLevel::DEBUG) << "listen socket success";
    }

    void serviceIO(int sockfd, InetAddr address)
    {
        // 长服务
        // // tcp socket 是 全双工的
        // LOG(LogLevel::DEBUG) << "client info is: " << address.ToString();   // 哪个客户端

        // while(true)
        // {
        //     char inbuffer[128] = { 0 };
        //     // 读取数据 —— ssize_t read(int fd, void buf[.count], size_t count)
        //     ssize_t n = read(sockfd, inbuffer, sizeof(inbuffer));
        //     if(n > 0)
        //     {
        //         inbuffer[n] = 0;    // 手动添加0，将其当作字符串
        //         LOG(LogLevel::INFO) << address.ToString() << " say# " << inbuffer;      // 显示客户端读取到的信息
        //         std::string echo_string = "server echo$ ";
        //         echo_string += inbuffer;

        //         // 写数据 —— ssize_t write(int fd, const void buf[.count], size_t count)
        //         write(sockfd, echo_string.c_str(), echo_string.length());
        //     }
        //     else if(n == 0)     // 读到0了，说明断开连接了，和管道通信类似
        //     {
        //         LOG(LogLevel::INFO) << "client quit, address: " << address.ToString();
        //         break;
        //     }
        //     else        // 小于0，表明读出错了
        //     {
        //         LOG(LogLevel::ERROR) << "read error, address: " << address.ToString();
        //         break;
        //     }
        // }

        // 短连接 短服务 —— 读完一段消息就退出
        char inbuffer[128] = {0};
        // 读取数据 —— ssize_t read(int fd, void buf[.count], size_t count)
        ssize_t n = read(sockfd, inbuffer, sizeof(inbuffer));
        if (n > 0)
        {
            inbuffer[n] = 0;                                                   // 手动添加0，将其当作字符串
            LOG(LogLevel::INFO) << address.ToString() << " say# " << inbuffer; // 显示客户端读取到的信息
            std::string echo_string = "server echo$ ";
            echo_string += inbuffer;

            // 写数据 —— ssize_t write(int fd, const void buf[.count], size_t count)
            write(sockfd, echo_string.c_str(), echo_string.length());
        }
        else if (n == 0) // 读到0了，说明断开连接了，和管道通信类似
        {
            LOG(LogLevel::INFO) << "client quit, address: " << address.ToString();
        }
        else // 小于0，表明读出错了
        {
            LOG(LogLevel::ERROR) << "read error, address: " << address.ToString();
        }

        close(sockfd);
    }

    class ThreadData
    {
    public:
        ThreadData(TcpServer *ts, int sockfd, InetAddr addr)
            : _this(ts), _sockfd(sockfd), _addr(addr)
        {
        }
        ~ThreadData() {}

    public:
        TcpServer *_this;
        int _sockfd;
        InetAddr _addr;
    };

    // 线程处理函数 static 修饰就是为了解决 this 指针的影响
    static void *threadRoutine(void *args)
    {
        ThreadData *td = static_cast<ThreadData *>(args);
        // 将自己设置成分离状态 —— int pthread_detach(pthread_t thread)
        pthread_detach(pthread_self());

        // 提供 IO 服务
        td->_this->serviceIO(td->_sockfd, td->_addr);

        delete td; // 释放掉td

        return nullptr;
    }

    void StartTcpServer()
    {
        // signal(SIGCHLD, SIG_IGN);   // 显示忽略 SIGCHLD 信号，最佳实践
        while (true)
        {
            // 1. 服务器获取链接 —— 调用系统调用：accept
            // int accept(int sockfd, struct sockaddr *_Nullable restrict addr, socklen_t *_Nullable restrict addrlen)
            struct sockaddr_in client_addr;
            socklen_t len = sizeof(client_addr);
            int sockfd = accept(_listensockfd, (struct sockaddr *)&client_addr, &len);
            if (sockfd < 0) // 获取新连接失败
            {
                LOG(LogLevel::WARNING) << "accept error";
                continue; // 继续连接
            }
            LOG(LogLevel::DEBUG) << "accept success: " << sockfd; // 获取新连接成功

            // 2. 处理新 sockfd —— 服务器没有办法并发的处理请求
            // version0:
            // InetAddr clientaddress(client_addr);
            // serviceIO(sockfd, clientaddress);       // 哪个客户端要通信

            // version1: 多进程处理新的 sockfd
            // 服务端获得请求，交给子进程取处理，父进程继续获取新连接
            // 网络通信的大原则：一个资源不用了，一定要尽早释放它。有用的且有限的就是资源。fd本身也是资源
            // 父子进程需要关闭自己不需要的文件描述符 fd，不然有风险
            // pid_t id = fork();
            // if(id < 0)
            // {
            //     LOG(LogLevel::FATAL) << "fork error";
            //     exit(FORK_ERR);
            // }
            // else if(id == 0)    // 子进程
            // {
            //     // 子进程关闭自己不需要的 fd，即 _listensockfd
            //     close(_listensockfd);

            //     // 子进程中创建子进程，创建完毕后自己退出
            //     if(fork() > 0) { exit(0); }

            //     // 这部分是孙子进程执行的
            //     // 孙子进程的父进程退出了，它就变成了孤儿进程，由系统领养，退出后，系统自动回收
            //     InetAddr clientaddress(client_addr);
            //     serviceIO(sockfd, clientaddress);       // 哪个客户端要通信
            //     close(sockfd);
            //     exit(0);
            // }
            // else    // 父进程
            // {
            //     // 父进程关闭自己不需要的 fd，即sockfd
            //     close(sockfd);
            //     // 子进程退出，父进程需要阻塞等待呀？这样逻辑不就又变成串行的吗？
            //     // 可以使用信号来解决，子进程退出，系统自动回收
            //     // 也可以在子进程中再fork创建子进程,让父进程去等待它的孩子,它的孩子一下就退出了，不需要阻塞等待
            //     waitpid(id, nullptr, 0);
            // }

            // // 3. 关闭新 sockfd
            // close(sockfd);

            // version3: 多线程处理新的sockfd
            // pthread_t tid;
            // InetAddr clientaddress(client_addr);
            // ThreadData *td = new ThreadData(this, sockfd, clientaddress);
            // // int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr,
            // // void *(*start_routine)(void *), void *restrict arg);
            // pthread_create(&tid, nullptr, threadRoutine, (void *)td); // 创建线程

            // 主线程不是还要等待吗？可以创建完线程之后，不用再等待新线程。将线程设置成分离状态，那么主线程就不需要管了
            // version3 版本存在问题：serviceIO 是一种长服务，要求对应的连接是长连接，只能处理小型应用。将 serviceIO 改成短服务
            // 每次服务时，都需要创建线程，可以直接使用线程池。

            // version4: 接入线程池处理新的 sockfd
            InetAddr clientaddress(client_addr);
            // 参数为空，返回值为空
            ThreadPool<task_t>::LazySingle()->PushQueue([this, sockfd, clientaddress]()->void{
                this->serviceIO(sockfd, clientaddress);
            });       // 获取单例,来个任务入队列
        }
    }

private:
    uint16_t _listensockfd;
    uint16_t _port;
};