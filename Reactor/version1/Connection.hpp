#pragma once

// 每个文件描述符都要有文件缓冲区
// 每次客户端向服务器发送TCP连接之后，最终服务器都会得到一个连接结构体（Connection），管理到TCP socket 中
// 在 OS 中存在多个文件描述符，每个文件描述符都有自己的缓冲区，所以就需要管理这些文件缓冲区
// “先描述，再组织”，因此先将连接描述起来

#include "Reactor.hpp"

#include <iostream>
#include <string>
#include <stdint.h>
#include <ctime>
#include <sys/epoll.h>


class BaseConnection
{
public:
    BaseConnection() :_sockfd(-1), _events(EPOLLIN), _active_time(time(nullptr)) {}
    BaseConnection(int sockfd, uint32_t event) :_sockfd(sockfd), _events(event), _active_time(time(nullptr)) {}
    ~BaseConnection() {}

    void Active()   // 连接活跃
    {
        _active_time = time(nullptr);   // 更新时间戳
    }

    virtual int Recver() = 0;       // 发
    virtual int Sender() = 0;       // 收
    virtual int Exceptioner() = 0;  // 异常
    
    int GetSockfd() { return _sockfd; }         // 获取 Connection 对象的文件描述符
    uint32_t GetEvent() { return _events; }     // 获取 Connection 对象所关心的事件

protected:
    int _sockfd;    // 文件描述符
    uint32_t _events;    // 所关心的事件
    std::string _inbuffer;      // 文件描述符的接收缓冲区
    std::string _outbuffer;     // 文件描述符的发送缓冲区
    time_t _active_time;      // 连接最近一次的活跃时间，做问题说明

public:
    Reactor *reactor;   // 回指指针
};

// class NormalConnection : BaseConnection
// {

// };

// class ListenConnection : BaseConnection
// {

// };