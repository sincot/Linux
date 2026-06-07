#pragma once

#include "Logger.hpp"
#include "Socket.hpp"
#include "Epoller.hpp"
#include "Connection.hpp"

#include <unordered_map>

using namespace LOGMOUDLE;
using namespace SOCKET_MOUDLE;


static const int grevent_num = 128;


/*******************************/
// 文件描述符分成两种 listen socket 和 normal socket
// normal socket 处理的事件：recv send exception
// listen socket 处理的事件：recv(accept) send(nullptr) exception(nullptr)
// listen socket 它本身就是一个连接
/*******************************/

// 现在实现的类，拥有多功能，它相当于一个管理连接的容器
// class TcpServer
class Reactor
{
public:
    // TcpServer(uint16_t port = gdefaultport) :_port(port)
    // {
    //     _listensock = std::make_unique<TCPSocket>();
    //     _epoller = std::make_unique<Epoller>();

    //     // // 将 listensock 设置到内核中
    //     // _epoller->EpollAddSocket(_listensock->GetSockfd(), EPOLLIN);    // 关心 EPOLLIN 事件

    //     // listensock 对应的就是连接，直接将 listensock 添加到 _connections 中，但是类型匹配不上
    // }

    // TcpServer()
    Reactor()
    {
        _epoller = std::make_unique<Epoller>();

        // // 将 listensock 设置到内核中
        // _epoller->EpollAddSocket(_listensock->GetSockfd(), EPOLLIN);    // 关心 EPOLLIN 事件

        // listensock 对应的就是连接，直接将 listensock 添加到 _connections 中，但是类型匹配不上
    }


    // ~TcpServer() {}
    ~Reactor() {}

    bool IsConnestionLegal(int sockfd)      // 判断事件是否合法
    {
        // 在 _connetions 容器中寻找是否存在 sockfd 对应的事件 
        // 不等于合法（true），等于非法（false）
        return _connections.find(sockfd) != _connections.end();
    }

    void Dispatcher(int num)
    {
        for(int i = 0; i < num; i++)
        {
            uint32_t revent = _revents[i].events;   // 就绪的事件
            int sockfd = _revents[i].data.fd;       // 就绪的 fd

            // 每个文件描述符都是一个 Connection 连接
            // 不在关心 socket 是 listensocket 还是 normalsocket
            if(revent & EPOLLERR || revent & EPOLLHUP) revent |= (EPOLLIN | EPOLLOUT);  // 统一异常处理
            if(revent & EPOLLIN && IsConnestionLegal(sockfd)) _connections[sockfd]->Recver();    
            if(revent & EPOLLOUT && IsConnestionLegal(sockfd)) _connections[sockfd]->Sender();

        }
    }

    void TcpServerRun()
    {
        int timeout = 1000; // 1s
        while(true)
        {
            int n = _epoller->EpollWait(_revents, grevent_num, timeout);      // 监测事件，获取就绪事件

            switch(n)
            {
                case 0:     // 超时
                    LOG(LogLevel::INFO) << "Time out...";
                    break;
                case -1:    // 错误
                    LOG(LogLevel::FATAL) << "epoll_wait error";
                    exit(EXIT_CODE::EPOLLWAIT_ERROR);
                    break;
                default:    // 正确
                    LOG(LogLevel::FATAL) << "epoll_wait success";
                    Dispatcher(n);       // 事件派发器 派发获取到的 n 个就绪事件

                    break;
            }
        }
    }

public:
    void AddConnection(std::shared_ptr<BaseConnection> conn)    // 添加连接
    {
        // 1. 将新连接设置到内核中，调用 epoller
        _epoller->EpollAddSocket(conn->GetSockfd(), conn->GetEvent());

        // 2. 将新连接托管到 _connections
        _connections[conn->GetSockfd()] = conn;

        // 3. 让 conn 对象中的 reactor 回指指针指向 Reactor 对象，也就是 this
        conn->reactor = this;   
    }

    void DelConnection()    // 删除连接
    {

    }

private:
    // 将 port 和 listensock 成员属性实现到 Listener 中
    // uint16_t _port;
    // std::unique_ptr<Socket> _listensock;    // 监听套接字

    std::unique_ptr<Epoller> _epoller;      // Epoller 对象，由它对事件做管理

    struct epoll_event _revents[grevent_num];

    // 将文件描述符与连接对象管理起来，使用 map 容器
    std::unordered_map<int, std::shared_ptr<BaseConnection>> _connections;
};