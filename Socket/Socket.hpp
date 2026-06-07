#pragma once

#include "InetAddr.hpp"
#include "Logger.hpp"

#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>



namespace SOCKET_MOUDLE    // 只需要设计TCP即可
{
    using namespace LOGMOUDLE;

    static const int gbacklog = 8;

    enum
    {
        SUCCESS = 0,
        SOCKET_ERR,
        BIND_ERR,
        LISTEN_ERR,
    };



    // 模板方法设计模式
    // 虚实结合，不同方法的差异化由底层实现，将共同的逻辑组合在一起
    // TCP/UDP 部分使用的接口都是一样的，既然如此不需要重复设计
    class Socket
    {
    public:
        ~Socket() {}
    
    public:
        // 尽管不知道是哪种套接字，它们都包含相同的接口
        // 创建套接字，绑定套接字，监听套接字，读数据，发送数据
        virtual void CreateSocket() = 0;      // 创建 socket 
        virtual void BindSocket(uint16_t port) = 0;        // 绑定 socket
        virtual void ListenSocket() = 0;      // 监听 socket
        virtual ssize_t RecvData() = 0;       // 读数据
        virtual ssize_t SendData() = 0;       // 发送数据

    public:
        void BuildTcpSocketMethod(uint16_t port)     // 模板方法
        {
            CreateSocket();                   // 创建 socket 
            BindSocket(port);                 // 绑定 socket
            ListenSocket();                   // 监听 socket
        }

        // void BuildUdpSocketMethod()     // 模板方法
        // {
        //     CreateSocket();      // 创建 socket 
        //     BindSocket();        // 绑定 socket
        // }
    };

    // TCPSocket 继承 Socket
    class TCPSocket : public Socket
    {
    public:
        // BuildTcpSocketMethod 方法天然就有，只需要实现差异化的接口
        TCPSocket() :_sockfd(0) {}

        void CreateSocket() override    // 创建 socket
        {
            _sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if(_sockfd < 0)
            {
                LOG(LogLevel::FATAL) << "create socket error";
                exit(SOCKET_ERR);
            }
        }

        void BindSocket(uint16_t port) override       // 绑定 socket
        {
            InetAddr addr(port);
            if(bind(_sockfd, addr.GetNetAddress(), addr.GetLen()) != 0)
            {
                LOG(LogLevel::FATAL) << "bind socket error";
                exit(BIND_ERR);
            }
        }
        void ListenSocket() override    // 监听 socket
        {
            if(listen(_sockfd, gbacklog) != 0)
            {
                LOG(LogLevel::FATAL) << "listen socket error";
                exit(LISTEN_ERR);
            }
        }

        ~TCPSocket() {}
    protected:
        int _sockfd;
    };

    // // UDPSocket 继承 Socket
    // class UDPSocket : public Socket
    // {
    // public:

    // public:
    // };
}