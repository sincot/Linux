#pragma once

#include "UserManager.hpp"
#include "Mutex.hpp"


#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 路由模块
class Route
{
public:
    Route() :_uma(std::make_unique<UserManager>()) {}
    ~Route() {}

    void CheckUser(const InetAddr &addr)    // 检查用户是否首次发送消息，如果是添加到用户列表
    {
        LockGuard lockguard(_mutex);
        _uma->AddUser(addr);
    }

    void OfflineUser(const InetAddr &addr)  // 用户下线，将该用户从用户列表中删除
    {
        LockGuard lockguard(_mutex);
        _uma->DeleteUser(addr);
    }

    // 将消息通过网络文件发送给用户列表中的所有的用户
    void Broadcast(int sockfd, const std::string &message)      
    {
        LockGuard lockguard(_mutex);        // 保护在线用户列表
        auto &users = _uma->GetUsers();     // 获取用户列表
        for(auto &user : users)
        {
            // 发送数据
            // ssize_t sendto(int sockfd, const void buf[.len], size_t len, int flags,
            //                const struct sockaddr *dest_addr, socklen_t addrlen);
            sendto(sockfd, message.c_str(), message.size(), 0, (sockaddr *)user.GetNetAddress(), user.GetLen());
        }
    }

private:
    std::unique_ptr<UserManager> _uma;       // 用户管理
    Mutex _mutex;                            // 保护在线用户列表
};