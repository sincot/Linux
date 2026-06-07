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
        ACCEPT_ERR,
    };

    // 模板方法设计模式
    // 虚实结合，不同方法的差异化由底层实现，将共同的逻辑组合在一起
    // TCP/UDP 部分使用的接口都是一样的，既然如此不需要重复设计
    class Socket
    {
    public:
        ~Socket() {}
    
    protected:
        // 尽管不知道是哪种套接字，它们都包含相同的接口
        // 创建套接字，绑定套接字，监听套接字，读数据，发送数据
        virtual void CreateSocket() = 0;      // 创建 socket 
        virtual void BindSocket(uint16_t port) = 0;        // 绑定 socket
        virtual void ListenSocket() = 0;      // 监听 socket

    public:
        void BuildTcpSocketMethod(uint16_t port)     // 模板方法
        {
            CreateSocket();                   // 创建 socket 
            BindSocket(port);                 // 绑定 socket
            ListenSocket();                   // 监听 socket
        }

        void BuildTcpClientMethod()
        {
            CreateSocket();
        }
        
    public:
        virtual std::shared_ptr<Socket> AcceptSocket(InetAddr &addr) = 0;
        virtual int GetSockfd() = 0;
        virtual ssize_t RecvData(std::string *out) = 0;
        virtual ssize_t SendData(const std::string &in) = 0; 
        virtual void CloseSockfd() = 0;
        virtual bool Connect(InetAddr &address) = 0;    // 发起连接
    };

    // TCPSocket 继承 Socket
    class TCPSocket : public Socket
    {
    public:
        // BuildTcpSocketMethod 方法天然就有，只需要实现差异化的接口
        TCPSocket() :_sockfd(0) {}
        TCPSocket(int sockfd) :_sockfd(sockfd) {}

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

        std::shared_ptr<Socket> AcceptSocket(InetAddr &clientaddr) override     // 获取新连接
        {
            struct sockaddr_in addr;
            socklen_t len = sizeof(addr);
            int sockfd = accept(_sockfd, CONV(&addr), &len);
            if(sockfd < 0)      // 获取套接字失败
            {
                LOG(LogLevel::WARNING) << "accept socket error";
                exit(ACCEPT_ERR);

                return nullptr;
            }

            clientaddr = addr;  // 将客户端的 addr 带出去

            return std::make_shared<TCPSocket>(sockfd);   // 基类指针返回派生类对象
        }

        int GetSockfd() override { return _sockfd; }    // 获取文件描述符

        ssize_t RecvData(std::string *out) override     // 接收数据
        {
            // recv: ssize_t recv(int sockfd, void buf[.len], size_t len, int flags)
            char inbuffer[10240];     // 10KB 的缓冲区
            ssize_t n = recv(_sockfd, inbuffer, sizeof(inbuffer) - 1, 0);
            if(n > 0)   // 读成功
            {
                inbuffer[n] = 0;
                *out = inbuffer;
            }   

            return n;
        }
        
        ssize_t SendData(const std::string &in) override     // 发送数据
        {
            // send: ssize_t send(int sockfd, const void buf[.len], size_t len, int flags)
            return send(_sockfd, in.c_str(), in.length(), 0);
        }

        void CloseSockfd()     // 关闭文件描述符
        {
            if(_sockfd >= 0) { close(_sockfd); }
        }
        
        bool Connect(InetAddr &address) override
        {
            // int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
            int n = connect(_sockfd, address.GetNetAddress(), address.GetLen());
            if(n < 0) { return false; }
            else { return true; }
        }

        ~TCPSocket() {}

    private:
        int _sockfd;
    };
}