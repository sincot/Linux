#pragma once

#include "Logger.hpp"
#include "Socket.hpp"

#include <iostream>
#include <sys/select.h>
#include <unistd.h>
#include <string>
#include <poll.h>

using namespace LOGMOUDLE;
using namespace SOCKET_MOUDLE;

static const int gfdnum = 1024;   // select 模型中最大的文件描述符的值加 1
static const int gdefaultfd = -1; // 默认的文件描述符值

/***** *****/
// 事件监听器 —— select
// 事件派发器 —— DispatchEvents
// 连接管理器 —— Accpter
// IO处理器 —— IOService
/***** *****/

class PollServer
{
public:
    explicit PollServer(uint16_t port = 8080)
        : _port(port), _isRunning(false)
    {
        _listenSocket = std::make_unique<TCPSocket>(); // 创建监听套接字
        _listenSocket->BuildTcpSocketMethod(port);     // 构建 TCP 服务器套接字
        LOG(LogLevel::INFO) << "create listensock success, fd: " << _listenSocket->GetSockfd();

        for (int i = 0; i < gfdnum; i++)
        {
            // struct poll 结构体做初始化
            _fdArray[i].fd = gdefaultfd;
            _fdArray[i].events = _fdArray[i].revents = 0;

        } // 初始化辅助数组

        // 默认将最开始的 fd 添加到管理数组中，存储到起始位置
        _fdArray[0].fd = _listenSocket->GetSockfd(); // 将监听套接字的 fd 添加到管理数组中
        _fdArray[0].events = POLLIN;    // 让系统关心 POLLIN 事件（事件可读）
    }

    ~PollServer() {}

    void Accepter()     // 连接管理器，进行连接处理
    {
        LOG(LogLevel::INFO) << "new connection coming"; // 有新连接来了
        InetAddr clientaddr;                            // 定义一个临时变量，获取新连接的客户端地址

        // 此时，就不会在 accept 这里阻塞了
        // 因为 select 已经告诉我们监听套接字就绪了，表示有新的连接来了，所以 accept 也不会阻塞了
        int sockfd = _listenSocket->AcceptSocket(clientaddr); // 获取新连接的套接字
        if (sockfd >= 0)                                      // 获取新连接成功了
        {
            LOG(LogLevel::INFO) << "accept new connection success, client: "
                                << clientaddr.ToString() << ", sockfd: " << sockfd;
        }

        // 建议不要使用，文件描述符映射在辅助数组中对应的位置
        // 虽然查找起来更加的简答，这样辅组数组只能存储 0~1023 文件描述符
        // 在映射时，需要额外判断文件描述符是否大于1023

        // 在辅助数组找空位置
        int pos = 0;
        for (; pos < gfdnum; pos++)
        {
            if (_fdArray[pos].fd == gdefaultfd)
            {
                break;
            } // 找到空位置了
        }
        if (pos == gfdnum) // 没有找到空位置，说明管理的文件描述符已经满了
        {
            // 管理的文件描述符已经满了，无法接受新的连接了
            LOG(LogLevel::ERROR) << "too many connections, cannot accept new connection";
            close(sockfd); // 关闭新的套接字
        }
        else
        {
            // 找到合法的位置未被占用，将新的套接字加入到辅助数组中
            _fdArray[pos].fd = sockfd;
            _fdArray[pos].events = POLLIN;      // 关心事件可读
            _fdArray[pos].revents = 0;
            LOG(LogLevel::INFO) << "new connection added to fd array, sockfd: " << sockfd;
        }
    }

    void IOService(int i)   // IO处理器，处理IO事件
    {
        char outbuffer[1024]; // 定义一个缓冲区，读取数据
        // 当前读数据,不会被阻塞，因为事件已经就绪了
        ssize_t n = recv(_fdArray[i].fd, outbuffer, sizeof(outbuffer), 0);
        if (n > 0)
        {
            outbuffer[n] = '\0'; // 将读到的数据转换成字符串
            LOG(LogLevel::INFO) << "read data from client# " << outbuffer;
            std::string echo_string;
            echo_string += outbuffer;
            // ssize_t send(int __fd, const void *__buf, size_t __n, int __flags)
            // 可以直接发，大部分文件描述符的读事件不一定是就绪的，写事件默认都是就绪的
            send(_fdArray[i].fd, echo_string.c_str(), echo_string.length(), 0);
        }
        else if (n == 0) // 对方将连接关闭了（退出也是事件就绪）
        {
            LOG(LogLevel::INFO) << "client quit, sock: " << _fdArray[i].fd;
            // 不想让 select 管理某个文件描述符了，将文件描述符从辅助数组中移除
            _fdArray[i].fd = gdefaultfd; // 设置成默认值

            // 关闭 fd
            close(_fdArray[i].fd);
        }
        else
        { 
            LOG(LogLevel::ERROR) << "recv errror: " << _fdArray[i].fd; 
            // 不想让 select 管理某个文件描述符了，将文件描述符从辅助数组中移除
            _fdArray[i].fd = gdefaultfd; // 设置成默认值

            // 关闭 fd
            close(_fdArray[i].fd);
        }
    }

    void DispatchEvents() // 处理事件
    {
        for (int i = 0; i < gfdnum; i++)
        {
            if (_fdArray[i].fd == gdefaultfd)
                continue; // 这个位置没有合法的文件描述符了，继续往下找
            else          // 文件描述符合法，但是是否就绪还需要判断
            {
                if (_fdArray[i].events & POLLIN) // 检测文件描述符的读事件是否就绪
                {
                    if (_fdArray[i].fd == _listenSocket->GetSockfd()) // 文件描述符是监听套接字
                    {
                       Accepter();      // 连接管理器
                    }
                    else // 普通文件描述符就绪了
                    {
                        IOService(i);   // 处理IO事件
                    }
                }
            }
        }
    }

    void PrintFd()
    {
        std::cout << "_fdArray have fds: ";
        for (int i = 0; i < gfdnum; i++)
        {
            if (_fdArray[i].fd == gdefaultfd)
                continue;
            std::cout << _fdArray[i].fd << " ";
        }
        std::cout << std::endl;
    }

    void PollRun() // 服务器运行
    {
        int timeout = 1000;     // 1000ms ~ 1s
        // readfds 是输入输出型参数，输入与输出会互相影响
        // 每次循环都要重新设置 readfds 的位图，因为 select 会修改 readfds 的位图，清空没有就绪的文件描述符
        while (!_isRunning) // 只要服务器没有退出，就一直运行
        {
            // 每次都打印辅助数组中存在的文件描述符
            // 来了新连接，就能看到辅组数组中的文件描述符在不断的变化
            PrintFd();

            // int poll(pollfd *__fds, nfds_t __nfds, int __timeout)
            int n = poll(_fdArray, sizeof(_fdArray) / sizeof(_fdArray[0]), timeout);
            switch (n)
            {
            case -1:
                LOG(LogLevel::ERROR) << "poll error"; // select 出错
                break;
            case 0:
                LOG(LogLevel::INFO) << "poll timeout"; // select 超时
                break;
            default:
                LOG(LogLevel::INFO) << "poll ready, n = " << n; // select 就绪, 有 n 个文件描述符就绪了

                // 事件派发器
                DispatchEvents(); // 不仅要处理新链接，还需要处理普通的事件

                break;
            }
        }
    }

private:
    uint16_t _port; // 端口号

    // 一个 tcp 服务器最开始的时候，只有一个fd，就是监听套接字的 fd，后续才会有更多的 fd 加入到 select 模型中
    // 对于 listensock，我们直接调用 accept，它本质也可以当做 IO 来
    // 应该将 listensock 的等待工作交给 select 来做，而不是直接调用 accept 来等待连接
    // 读事件就绪了就表示有新的连接来了，这时才调用 accept 来获取新连接的套接字
    std::unique_ptr<Socket> _listenSocket; // 监听套接字
    bool _isRunning;                       // 服务器是否正在运行,flase表示没有运行，true 表示正在运行

    struct pollfd _fdArray[gfdnum];
};