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

using task_t = std::function<void()>;                      // 返回值为void，参数为空
using handler_t = std::function<std::string(std::string)>; // 参数是要处理的字符串命令，参数是处理的结果
using HandlerTips_t = std::function<std::string()>;


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

    void Register(handler_t handler, HandlerTips_t handlerTips)     // 注册服务
    {
        _handler = handler;
        _handlerTips = handlerTips;
    }

    int HandlerIO(int sockfd, InetAddr address) // 处理 IO 工作
    {
        char inbuffer[128] = {0};
        // 将字符串 "ls -a -l" 交给上层处理，设置一个函数，专门来处理

        // 读取数据 —— ssize_t read(int fd, void buf[.count], size_t count)
        ssize_t n = read(sockfd, inbuffer, sizeof(inbuffer) - 1);
        if (n > 0)
        {
            inbuffer[n] = 0; // 手动添加0，将其当作字符串
            // 回调处理
            std::string result;
            if (nullptr != _handler)
            {
                result = _handler(inbuffer);
            }
            else
            {
                LOG(LogLevel::WARNING) << "server init not ready " << address.ToString();
            }

            // 写数据 —— ssize_t write(int fd, const void buf[.count], size_t count)
            write(sockfd, result.c_str(), result.length());
        }
        else if (n == 0) // 读到0了，说明断开连接了，和管道通信类似
        {
            LOG(LogLevel::INFO) << "client quit, address: " << address.ToString();
        }
        else // 小于0，表明读出错了
        {
            LOG(LogLevel::ERROR) << "client read error, address: " << address.ToString();
        }

        return n;
    }

   void ReturnTips(int sockfd, InetAddr address)
   {
        // 获取命令行信息
        std::string result = _handlerTips();
        write(sockfd, result.c_str(), result.length());
   }


    // 线程来处理的
    void serviceIO(int sockfd, InetAddr address)
    {
        // 长服务
        // tcp socket 是 全双工的
        LOG(LogLevel::DEBUG) << "client info is: " << address.ToString(); // 哪个客户端

        while (true)
        {
            // 写回命令行
            ReturnTips(sockfd, address);
            // 处理 IO
            if(HandlerIO(sockfd, address) <= 0)     // 判断处理的结果
                break;
        }
    }

    void StartTcpServer()
    {
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

            // version4: 接入线程池处理新的 sockfd
            InetAddr clientaddress(client_addr);
            ThreadPool<task_t>::LazySingle()->PushQueue([this, sockfd, clientaddress]()
                                                        { this->serviceIO(sockfd, clientaddress); });
        }
    }

private:
    uint16_t _listensockfd;
    uint16_t _port;

    handler_t _handler;
    HandlerTips_t _handlerTips;            // 获取命令行信息
};