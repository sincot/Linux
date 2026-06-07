#pragma once

#include "Logger.hpp"
#include "Mutex.hpp"
#include "InetAddr.hpp"


#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <string>
#include <strings.h>    // bzero 函数
#include <functional>


using namespace LOGMOUDLE;

const static int default_fd = -1;           // 文件描述符的默认值
const static int default_port = 8080;       // 默认端口号

// 处理新地址
using handler_addr_t = std::function<void (const InetAddr &)>;     // 返回值为 string，参数类型也为 string
using handler_msg_t = std::function<void(int sockfd, std::string msg)>;


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
        InetAddr local(_port);

        // 3. 将ip地址和port设置到内核中，即绑定套接字信息
        // 调用 bind 系统调用：int bind(int sockfd，const struct sockaddr *addr，socklen_t addrlen)
        int n = bind(_sockfd, (struct sockaddr*)(local.GetNetAddress()), local.GetLen());
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "bind sockrt error";
            exit(BIND_ERR);
        }
        // LOG(LogLevel::INFO) << "bind socket success, ip: " << _ip << ", port: " << _port;
        LOG(LogLevel::INFO) << "bind socket success, port: " << _port;
    }

    // 注册服务
    void RegisterService(handler_addr_t handler_addr, handler_msg_t handler_msg)
    {
        _handler_addr = handler_addr;
        _handler_msg = handler_msg;
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
                inbuffer[n] = 0;
                // 1. 收到消息时，检测用户是否是新用户 
                InetAddr client_address(peer);      // 构造客户端
                std::string tips = client_address.ToString();
                std::string message = tips + inbuffer;
                LOG(LogLevel::DEBUG) << message;
                _handler_addr(client_address);      // 将客户端的地址交给上层处理

                // 2. 转发消息
                _handler_msg(_sockfd, message);
            }
            else
            {
                LOG(LogLevel::ERROR) << "recvfrom error";
            }
        }
    }

private:
    int _sockfd;        // 打开的网络文件的文件描述符，未来通信的句柄
    uint16_t _port;     // 服务器的port，必须是固定的，不然客户端找不到
    handler_addr_t _handler_addr;              // 处理地址
    handler_msg_t _handler_msg;                // 处理消息
};