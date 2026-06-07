#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <strings.h>    // bzero 函数

// 描述客户端 —— 先描述
class InetAddr
{
public:
    // 用户传递客户端的地址的方式不同
    InetAddr(const struct sockaddr_in &address) :_address(address), _len(sizeof(address))
    {
        _ip = inet_ntoa(_address.sin_addr);     // 将4字节ip转成字符串ip
        _port = ntohs(_address.sin_port);       // 网络序列转主机序列
    }

    // "0.0.0.0" 就表示任意 ip 地址
    InetAddr(uint16_t port, const std::string &ip = "0.0.0.0") :_ip(ip), _port(port)
    {
        // 主机转网络
        // 将结构体 local 中的变量清 0，调用 bzero，引用头文件<strings.h>
        // void bzero(void *__s, size_t __n)
        bzero(&_address, sizeof(_address));

        _address.sin_family = AF_INET;

        // 这里接收到的 ip 是字符串类型的，需要将其转成4字节的，并且还要转成网络序列
        // 表示任意 ip 地址绑定
        _address.sin_addr.s_addr = INADDR_ANY;     // 最佳实践

        // 需要主机序列转化成网络序列：h->n
        // 调用 htons 函数，uint16_t htons(uint16_t __hostshort)
        _address.sin_port = htons(_port);

        _len = sizeof(_address);
    }

    InetAddr() {}   // 定义临时变量

    // 获取客户端的网络地址
    struct sockaddr_in *GetNetAddress()
    {
        return &_address;
    }

    // 获取客户端 socket 的长度
    socklen_t GetLen()
    {
        return _len;
    }

    // 重载 == 操作符   判断添加的用户是否与原用户相等
    bool operator == (const InetAddr &addr)
    {
        if(this == &addr) { return true; }
        return (this->_ip == addr._ip) && (this->_port == addr._port);
    }

    std::string ToString()
    {
        return "[" + _ip + ":" + std::to_string(_port) + "]";
    }
    ~InetAddr() {}

private:
    // net address
    struct sockaddr_in _address;
    socklen_t _len;      // socket 的长度
    // host address
    std::string _ip;    // 客户端的 ip 地址
    uint16_t _port;     // 客户端的 port 地址
};