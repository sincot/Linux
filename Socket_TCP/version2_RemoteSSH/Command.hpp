#pragma once

#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <pwd.h>
#include <unistd.h>

class Command
{
private:
    void LoadCommand()  // 白名单
    {
        _whitelist.emplace_back("ls -a -l");
        _whitelist.emplace_back("ls -l");
        _whitelist.emplace_back("pwd");
        _whitelist.emplace_back("whoami");
        _whitelist.emplace_back("who");
        _whitelist.emplace_back("ps -al");
        _whitelist.emplace_back("netstat -antp");
    }

    bool CommandIsSafe(const std::string &cmd)      // 判断命令是否安全
    {
        for(auto &e : _whitelist)
        {
            if(e == cmd) { return true; }
        }

        return false;
    }

public:
    Command() {}
    ~Command() {}

    std::string Excute(const std::string &cmd)
    {
        // 判断命令是否安全
        if(!CommandIsSafe(cmd)) { return "ban cmd"; }

        // 调用 popen 函数：FILE *popen(const char *command, const char *type)
        FILE *fp = popen(cmd.c_str(), "r");
        if(fp == nullptr)   // 打开失败
        {
            return "excute error";
        }

        std::string result;
        char buffer[1024];
        while(fgets(buffer, sizeof(buffer), fp))    // 读取文件中的字符串
        {
            result += buffer;
        }

        pclose(fp);     // 关闭打开的文件

        return result;
    }
    
    std::string GetCommandString()
    {
        // xay@hcss-ecs-3ea8:~/code/Socket_TCP/version2_RemoteSSH$
        // 获取用户名
        struct passwd *pw = getpwuid(getuid());
        std::string username = (pw ? pw->pw_name : "unknown");
        // 获取主机名
        char hostname[128];
        if(gethostname(hostname, sizeof(hostname)) != 0)
        {
            snprintf(hostname, sizeof(hostname), "unknown");
        }
        // 获取当前工作路径
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd)) == nullptr)
        {
            snprintf(cwd, sizeof(cwd), "unknown");
        }


        return username + "@" + hostname + ":" + cwd + "$ ";
    }

private:
    // 设置一个指令白名单，只有在名单中的指令才能执行
    std::vector<std::string> _whitelist;
};