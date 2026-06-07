#pragma once

#include "InetAddr.hpp"
#include "Logger.hpp"

#include <iostream>
#include <string>
#include <vector>


using namespace LOGMOUDLE;


// 管理用户，用户在线列表，使用 ip+port 用来标识用户，用户就是 inetaddr
class UserManager
{
public:
    UserManager() {}

    void AddUser(const InetAddr &addr)      // 添加用户
    {
        if(SearchUser(addr)) { return; }    // 如果当前用户存在，就不需要添加了
        // 将客户端的 ip+port 添加到在线的用户列表里
        LOG(LogLevel::INFO) << "新增一个在线用户 ";
        _users.emplace_back(addr);  // 新增用户
    }

    bool SearchUser(const InetAddr &addr)      // 查找用户
    {
        for(auto &user : _users)
        {
            if(user == addr) { return true; }
        }

        return false;
    }

    void DeleteUser(const InetAddr &addr)       // 删除用户
    {
        for(auto it = _users.begin(); it != _users.end(); it++)
        {
            if(*it == addr)
            {
                _users.erase(it);
                break;
            }
        }
    }

    std::vector<InetAddr> &GetUsers()  // 获取当前的用户列表
    {
        return _users;
    }

    ~UserManager() {}

private:
    std::vector<InetAddr> _users; 
};