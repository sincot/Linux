#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void Usage(const std::string &process)
{
    std::cout << "Usage\n\t" << process;
    std::cout << " server_ip server_port" << std::endl;
}

// 客户端 ./client_udp server_ip server_port
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(2);
    }
    // 获取用户输入的服务端的 ip 和 port
    // 但是客户端怎么知道服务端的 ip 和 port？服务器的 port+ip 内置在客户端！！
    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);


    // 1. 创建 socket: int socket(int domain, int type, int protocol)
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        std::cerr << "create socket failed" << std::endl;
        exit(1);
    }

    // 2. 构建 client socket 信息
    // client 需要有自己的 ip 和 port，因为就是基于 socket 进行通信的
    // client 客户端不需要显示 bind 自己的 ip 和 port
    // 1. 为什么不让客户端显示的 bind? 
    // 服务端显示bind，需要传参，既然要传参就需要填参，既然要填参就需要指明ip和port
    // 在服务器中，端口号 port 必须是固定的，必须能够被客户端找到
    // 对于客户端，client 的 port 具体是多少，并不重要，只要具有唯一性即可
    // client 显示绑定 port，会出现 port 冲突
    // 2. 如何设置自己的 IP 和 port？
    // client 一般采用随机端口的方式，由 OS 自主选择
    // OS 如何自主选择？udp client 首次发送数据的时候，OS 底层会隐式的自动获取随机端口，然后绑定 port+ip

    struct sockaddr_in server;
    // 将结构体中的字段置为0，void *memset(void *__s, int __c, size_t __n)
    memset(&server, 0, sizeof(0));
    // 填充信息
    server.sin_family = AF_INET;
    // 需要将 ip 转成 4 字节，并且转成网络序列 h->n
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    // 转成网络序列 h -> ns
    server.sin_port = htons(server_port);

    while(true)
    {
        std::string message;
        // 1. 获取用户输入的数据
        std::cout << "Please Enter# ";
        std::getline(std::cin, message);

        // 2. client 客户端发送数据给 server 服务器，首次发送自动 bind
        // 现在不知道 server 是谁，因此需要用户传递
        // ssize_t sendto(int sockfd, const void buf[.len], size_t len, int flags,
        //                const struct sockaddr *dest_addr, socklen_t addrlen);
        ssize_t n = sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr *)&server, sizeof(server));
        if(n > 0)
        {
            // 接收消息
            // ssize_t recvfrom(int sockfd, void buf[restrict .len], size_t len, int flags, 
            // \ struct sockaddr *_Nullable restrict src_addr, socklen_t *_Nullable restrict addrlen);
            char inbuffer[128] { 0 };
            struct sockaddr_in tmp;
            socklen_t len = sizeof(tmp);
            ssize_t m = recvfrom(sockfd, inbuffer, sizeof(inbuffer), 0, (struct sockaddr *)&tmp, &len);
            if(m > 0)       // 读取成功
            {
                inbuffer[m] = 0;    // 手动添加 \0
                std::cout << inbuffer << std::endl;
            }
        }
    } 


    return 0;
}