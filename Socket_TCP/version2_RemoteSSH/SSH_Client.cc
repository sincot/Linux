#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <string>
#include <unistd.h>

#include "Logger.hpp"
#include "InetAddr.hpp"


using namespace LOGMOUDLE;

void Usage(const std::string &process)
{
    std::cerr << "Usage\n\t";
    std::cerr << process << " server_ip server_port " << std::endl;
}

// ./client_tcp server_ip server_port
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    // 获取服务端的 ip 和 port
    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    // 客户端执行的动作

    // 1. 创建 socket 套接字 —— int socket(int domain, int type, int protocol)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)  // 创建套接字失败
    {
        LOG(LogLevel::FATAL) << "create socket error";
        exit(2);
    }

    // 客户端要不要显示 bind 本地 ip+port。不需要。
    // OS 会自动帮助我们进行随机端口 bind，防止端口号冲突
    // 2. bind port 和 ip，发起建立连接的工作
    // 调用 connect 系统调用
    // int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
    InetAddr serveraddress(server_port, server_ip);
    int n = connect(sockfd, (struct sockaddr *)serveraddress.GetNetAddress(), serveraddress.GetLen());
    if(n < 0)
    {
        LOG(LogLevel::FATAL) << "connect to " << serveraddress.ToString() << "failed";
        exit(3);
    }

    // 3. 通信
    while(true)
    {
        char commandline[1024];
        std::string commandlinestr;
        // ssize_t read(int __fd, void *__buf, size_t __nbytes)
        ssize_t n = read(sockfd, commandline, sizeof(commandline));     // 从网络读取数据
        if(n > 0)
        {
            commandline[n] = 0;
            commandlinestr = commandline;
        }
        else
        {
            commandlinestr = "unknown@unknown:unknown$ ";
        }
        std::cout << commandlinestr;    // 打印命令行信息

        // 处理用户输入的命令
        std::string command;
        std::getline(std::cin, command);
        // 向指定的服务器发送消息
        // 写数据 —— ssize_t write(int fd, const void buf[.count], size_t count)
        write(sockfd, command.c_str(), command.size());

        // 读数据 —— ssize_t read(int fd, void buf[.count], size_t count);
        char inbuffer[128];
        ssize_t m = read(sockfd, inbuffer, sizeof(inbuffer));
        if(m > 0)   // 读取成功
        {
            inbuffer[m] = 0;
            LOG(LogLevel::DEBUG) << "read success: " << inbuffer; 
        }
        else if(m == 0)
        {
            LOG(LogLevel::DEBUG) << "read end of file";
            break;
        }
        else
        {
            LOG(LogLevel::ERROR) << "read error";
        }
    }


    return 0;
}