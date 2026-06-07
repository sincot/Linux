#pragma once

#include "Logger.hpp"
#include "Mutex.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <string>
#include <strings.h>    // bzero 函数

using namespace LOGMOUDLE;

const static int default_fd = -1;           // 文件描述符的默认值
const static int default_port = 8080;       // 默认端口号
// 错误码表
enum
{
    SUCCESS = 0,
    SOCKET_ERR,     // 创建套接字失败
    Usage_ERR,      // Usage 函数使用错误
    BIND_ERR,       // 绑定失败
};

class UdpServer
{
public:
    // UdpServer(const std::string ip, uint16_t port = default_port)
    //     : _ip(ip), _port(port), _sockfd(default_fd) 
    // {}

    UdpServer(uint16_t port = default_port): _port(port), _sockfd(default_fd) 
    {}

    ~UdpServer() 
    {
        // 关闭打开的网络文件描述符
        close(_sockfd);
    }

    void InitUdpServer()    // 初始化服务器
    {
        // 1. 创建套接字 —— 本质就是打开网络文件，具备系统特性，系统中存在了创建的套接字
        // int socket(int domain, int type, int protocol)
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);   // 网络 UDP

        if(_sockfd < 0)
        {
            LOG(LogLevel::FATAL) << "create socket error";     // 一个致命的错误
            exit(SOCKET_ERR);
        }
        LOG(LogLevel::INFO) << "create socket success, sockfd: " << _sockfd;

        // 2. 填充网络信息 —— 服务器的IP和port
        // 使用 sockadd_in 结构体需要包含头文件 <netinet/in.h> <arpa/inet.h> <sys/socket.h>
        struct sockaddr_in local;
        // 将结构体 local 中的变量清 0，调用 bzero，引用头文件<strings.h>
        // void bzero(void *__s, size_t __n)
        bzero(&local, sizeof(local));

        // 填充信息,并没有将IP和port设置到打开的网络文件中
        // struct sockaddr_in 就是一个数据类型，local 位于用户栈上
        local.sin_family = AF_INET;

        // 这里接收到的 ip 是字符串类型的，需要将其转成4字节的，并且还要转成网络序列
        // 调用 inet_addr, in_addr_t inet_addr(const char *cp)
        // local.sin_addr.s_addr = inet_addr(_ip.c_str());

        // 表示任意 ip 地址绑定
        local.sin_addr.s_addr = INADDR_ANY;     // 最佳实践

        // 需要主机序列转化成网络序列：h->n
        // 调用 htons 函数，uint16_t htons(uint16_t __hostshort)
        local.sin_port = htons(_port);

        // 3. 将ip地址和port设置到内核中，即绑定套接字信息
        // 调用 bind 系统调用：int bind(int sockfd，const struct sockaddr *addr，socklen_t addrlen)
        int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "bind sockrt error";
            exit(BIND_ERR);
        }
        // LOG(LogLevel::INFO) << "bind socket success, ip: " << _ip << ", port: " << _port;
        LOG(LogLevel::INFO) << "bind socket success, port: " << _port;
    }


    void RunUdpServer()     // 启动服务器
    {
        // 双方传递的是字符串
        char inbuffer[128];
        // 服务器是一个死循环
        while(true)
        {
            // 调用 recvfrom 函数
            // ssize_t recvfrom(int sockfd, void buf[restrict .len], size_t len, int flags, 
            // \ struct sockaddr *_Nullable restrict src_addr, socklen_t *_Nullable restrict addrlen);
            struct sockaddr_in peer;        // 远端的套接字信息
            socklen_t len = sizeof(peer);   // 结构体的大小
            // 接收用户发来的数据 —— 对应参数：void buf[restrict .len], size_t len
            // 用户的 socket 信息 —— 对应参数：struct sockaddr *_Nullable restrict src_addr, socklen_t *_Nullable restrict addrlen
            ssize_t n = recvfrom(_sockfd, inbuffer, sizeof(inbuffer)-1, 0, (struct sockaddr *)&peer, &len);
            if(n > 0)
            {
                uint16_t client_port = ntohs(peer.sin_port);       // 客户端的端口号,网络序列转成客户端序列
                // 1. n -> hs    2.4字节 -> 字符串 调用 inet_ntoa，char *inet_ntoa(struct in_addr in)
                std::string client_ip = inet_ntoa(peer.sin_addr);             
                std::string client_address = "[" + client_ip + ":" + std::to_string(client_port) + "]# ";

                // 1. 用户发来的数据
                inbuffer[n] = 0;    // 手动加上 \0
                LOG(LogLevel::DEBUG) << client_address << inbuffer;

                // 模拟出字符串经过服务端处理
                std::string echo_string = "server echo@ ";
                echo_string += inbuffer;

                // 将消息发送给客户端
                // ssize_t sendto(int sockfd, const void buf[.len], size_t len, int flags,
                //                const struct sockaddr *dest_addr, socklen_t addrlen);
                // 用户的 ip+port 存储在 peer 结构体中
                // 是 recvfrom 从网络中获取到的数据，遵守的是网络序列，即大端序列
                sendto(_sockfd, echo_string.c_str(), echo_string.length(), 0, (struct sockaddr *)&peer, len);
            }
            else
            {
                LOG(LogLevel::ERROR) << "recvfrom error";
            }
        }
    }

private:
    int _sockfd;    // 打开的网络文件的文件描述符，未来通信的句柄

    // 存在两种形态：点字符串 —— 192.128.2.2；4字节IP
    // 在内核中不会传递点字符串类型的IP，而是4字节IP
    // 需要将 4字节IP 转成 点字符串IP
    // std::string _ip;    // 服务器的IP地址
    uint16_t _port;     // 服务器的port，必须是固定的，不然客户端找不到
};