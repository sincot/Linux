#pragma once

#include "Logger.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"

#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <memory>
#include <string>

using namespace LOGMOUDLE;
using namespace SOCKET_MOUDLE;

const static uint16_t defaultport = 8080;
const static int gsize = 1024;
const static int gmaxevents = 8;

class EpollServer
{
public:
    EpollServer(uint16_t port = defaultport) 
        :_port(port), _listensock(std::make_unique<TCPSocket>()), _epfd(-1), _isrunning(false)
    {
        // 1. 创建 listensock
        _listensock->BuildTcpSocketMethod(port);
        LOG(LogLevel::INFO) << "create listensock success: " << _listensock->GetSockfd();       // 文件描述符为3

        // 2. 创建 epoll 模型
        _epfd = epoll_create(gsize);
        if(_epfd < 0) 
        {
            LOG(LogLevel::FATAL) << "create epoll error, return: " << _epfd;
            return; 
        }
        
        LOG(LogLevel::INFO) << "create epoll success, epfd is " << _epfd;       // 文件描述符为4

        // 将 listensock 添加到 epoll 中
        // int epoll_ctl(int __epfd, int __op, int __fd, epoll_event *)
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = _listensock->GetSockfd();
        // 向当前的红黑树新增结点
        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock->GetSockfd(), &event);
        if(n == 0)  // 等于0，说明添加成功
        {
            LOG(LogLevel::INFO) << "add listen sock to epoll success";
        }
    }

    ~EpollServer() {}

    void Listener()     // 连接管理器
    {
        InetAddr clientaddr;
        int newsockfd = _listensock->AcceptSocket(clientaddr);  // 获取新连接
        if(newsockfd < 0)   return;     // 获取新连接失败

        // 获取新连接成功
        LOG(LogLevel::INFO) << "accept success, new sockfd: " << newsockfd << " " << clientaddr.ToString();

        // 这里还不能直接读取新连接，新连接数据是否就绪还未知，应该加入 rb 中
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = newsockfd;
        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, newsockfd, &event);
    }

    void IOService(int sockfd)    // IO 处理器
    {
        char outbuffer[1024];
        // ssize_t recv(int __fd, void *__buf, size_t __n, int __flags)
        ssize_t n = recv(sockfd, outbuffer, sizeof(outbuffer), 0);      // 读取一次数据，不会阻塞
        if(n > 0)   // 读到了数据
        {
            outbuffer[n] = '\0';
            LOG(LogLevel::INFO) << "outbuffer: " << outbuffer;

            std::string echo_string = "echo# ";
            echo_string += outbuffer;

            // ssize_t send(int __fd, const void *__buf, size_t __n, int __flags)
            // 当前可以直接发送，因为fd的写事件默认都是就绪的
            send(sockfd, outbuffer, sizeof(outbuffer), 0);
        }
        else if(n == 0)     // 对端关闭
        {
            LOG(LogLevel::INFO) << "client quit: " << sockfd;

            // 注意：必须先将fd从epoll中移除，再关闭文件描述符fd
            // epoll_ctl 要删除对特定 fd 的关心，前提是fd必须在系统中是合法的
            // 将文件描述符从 rb 中删除,不让 epoll 关心fd
            struct epoll_event event;
            epoll_ctl(_epfd, EPOLL_CTL_DEL, sockfd, nullptr);   // 不需要关心是什么事件
            // 关闭文件描述符
            close(sockfd);
        }
        else    // 错误 
        {
            LOG(LogLevel::ERROR) << "recv error: " << sockfd;

            // 将文件描述符从 rb 中删除,不让 epoll 关心fd
            struct epoll_event event;
            epoll_ctl(_epfd, EPOLL_CTL_DEL, sockfd, nullptr);   // 不需要关心是什么事件
            // 关闭文件描述符
            close(sockfd);
        }
    }   

    void HanderEvent(struct epoll_event* revent, int n)
    {
        for(int i = 0; i < n; i++)      // 就绪几个就处理几个
        {
            uint32_t revents = revent[i].events;    // 就绪的事件
            int sockfd = revent[i].data.fd;         // 就绪的文件描述符

            // 判断是什么事件就绪，是读事件就绪，还是写事件就绪
            if(revents & EPOLLIN)   // 读事件就绪
            {
                if(sockfd == _listensock->GetSockfd())  // 新的连接事件
                {
                    Listener();     // 连接管理器
                }
                else    // 普通文件描述符就绪
                {
                    IOService(sockfd);    // IO 处理器
                }
            }
            // else if(revents & EPOLLOUT) // 写事件就绪
            // {

            // }
            // else if(revents & EPOLLERR)   // 错误
            // {

            // }
        }
    }

    void EpollRun()
    {
        int timeout = -1; 
        // int timeout = 1000;
        while(!_isrunning)
        {
            struct epoll_event revent[gmaxevents];
            // 从就绪队列中获取就绪队列
            // int epoll_wait(int __epfd, epoll_event *__events, int __maxevents, int __timeout)
            int n = epoll_wait(_epfd, revent, gmaxevents, timeout);
            if(n > 0)   // 存在事件就绪了
            {
                LOG(LogLevel::DEBUG) << "epoll_wait success";

                // 处理事件 —— 事件全部在 revent 中,就绪事件数为n
                HanderEvent(revent, n);
            }
            else if(n == 0)     // 超时了
            {
                LOG(LogLevel::INFO) << "time out...";
                break;
            }
            else        // 出错了
            {
                LOG(LogLevel::ERROR) << "epoll_wait error";
                break;
            }
        }
    }

private:
    std::unique_ptr<Socket> _listensock;    // listen 套接字
    uint16_t _port;      // 服务器端口号

    int _epfd;      // 创建的epoll模型的文件描述符
    bool _isrunning;    // 进程是否允许着，默认进程没有运行
};