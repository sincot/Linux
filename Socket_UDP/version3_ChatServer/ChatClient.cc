#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "Thread.hpp"
#include "InetAddr.hpp"

using namespace AY;

int sockfd = 0;
std::string server_ip;
uint16_t server_port;
std::string sendName;   // 发送数据方的名称

void Usage(const std::string &process)
{
    std::cout << "Usage\n\t" << process;
    std::cout << " server_ip server_port" << std::endl;
}

void RecverMessage() // 接收消息
{
    while (true) // 不断的接收消息
    {
        // 接收消息
        char inbuffer[128] = { 0 };
        struct sockaddr_in tmp;
        socklen_t len = sizeof(tmp);
        // ssize_t recvfrom(int sockfd, void buf[restrict .len], size_t len, int flags,
        // \ struct sockaddr *_Nullable restrict src_addr, socklen_t *_Nullable restrict addrlen);
        ssize_t m = recvfrom(sockfd, inbuffer, sizeof(inbuffer), 0, (struct sockaddr *)&tmp, &len);
        if (m > 0) // 读取成功
        {
            inbuffer[m] = 0; // 手动添加 \0
            std::cout << inbuffer << std::endl;
        }
    }
}

static void Online(InetAddr &serveraddr)
{
    std::cout << "Please Set Your Name# ";
    std::getline(std::cin, sendName);
    // 在启动客户端时，自动发送一条消息，表明客户端被启动了
    std::string online_message = sendName + " online";
    sendto(sockfd, online_message.c_str(), online_message.size(), 0,
          (struct sockaddr *)serveraddr.GetNetAddress(), serveraddr.GetLen());
}


void SendMessage() // 发送消息
{
    InetAddr serveraddr(server_port, server_ip);
    Online(serveraddr);

    while (true) // 不断的发消息
    {
        std::string message;

        // 获取用户输入
        std::cout << "Please Enter# ";
        std::getline(std::cin, message);

        // 每条发送的消息都携带客户端用户的名称
        message = sendName + " -> " + message;

        // client 发送数据给 server,首次发送机自动绑定
        sendto(sockfd, message.c_str(), message.size(), 0,
               (struct sockaddr *)serveraddr.GetNetAddress(), serveraddr.GetLen());
    }
}

// 客户端 ./client_udp server_ip server_port
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(2);
    }
    // 获取用户输入的服务端的 ip 和 port
    // 但是客户端怎么知道服务端的 ip 和 port？服务器的 port+ip 内置在客户端！！
    server_ip = argv[1];
    server_port = std::stoi(argv[2]);

    // 1. 创建 socket: int socket(int domain, int type, int protocol)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "create socket failed" << std::endl;
        exit(1);
    }

    // 创建线程
    Thread recver(RecverMessage);
    Thread send(SendMessage);

    // 启动线程
    recver.StartThread();
    send.StartThread();

    // 等待线程
    recver.JoinThread();
    send.JoinThread();

    return 0;
}