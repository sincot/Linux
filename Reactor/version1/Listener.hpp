#pragma once

/******************** 连接管理器 *****************/
#include <iostream>
#include <memory>
#include <sys/epoll.h>

#include "Connection.hpp"
#include "Socket.hpp"
#include "Exitcode.hpp"
#include "IOManager.hpp"

using namespace SOCKET_MOUDLE;

static const uint16_t gdefaultport = 8080;

class Listener : public BaseConnection
{
public:
    Listener(uint16_t port = gdefaultport) :_port(port)
    {
        _listensock = std::make_unique<TCPSocket>();
        _sockfd = _listensock->GetSockfd();
        _events = EPOLLIN;
        _listensock->BuildTcpSocketMethod(_port);
    }

    ~Listener() {}

    int Recver() override
    {
        // LOG(LogLevel::DEBUG) << "Listener recver";
        
        InetAddr clientaddr;
        int sockfd = _listensock->AcceptSocket(clientaddr);
        if(sockfd < 0)
        {
            LOG(LogLevel::ERROR) << "accept error";
            exit(EXIT_CODE::ACCEPT_ERROR);
        }

        // 获取了新连接，将该连接包装成为一个 connection
        std::shared_ptr<BaseConnection> conn = std::make_shared<IOManager>(sockfd, EPOLLIN);   // 基类指针构建派生类对象

        // 将 IOManager 设置到 Reactor 中，怎么通过 Listener 找到 Reactor?
        // Listener 类就包含在 Reactor 中，可以使用回指指针，实现在 Connection 类中
        reactor->AddConnection(conn);


        return 0;
    }

    int Sender() override
    {
        return 0;
    }

    int Exceptioner() override
    {
        return 0;
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;    // 监听套接字

    // int _sockfd;    // 文件描述符
    // uint32_t _events;    // 所关心的事件
    // std::string _inbuffer;      // 文件描述符的接收缓冲区
    // std::string _outbuffer;     // 文件描述符的发送缓冲区
    // time_t _active_time;      // 连接最近一次的活跃时间，做问题说明
};