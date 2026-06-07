#include "Protocol.hpp"
#include "Socket.hpp"

#include <memory>
#include <iostream>
#include <time.h>

using namespace SOCKET_MOUDLE;

// netcal_client 目标IP 目标主机端口号
static void Usage(const std::string &process)
{
    std::cout << "Usage:\n\t" << process << " server_ip server_port" << std::endl;
}

static void HandlerResponse(Response &resp)
{
    // 打印应答结果
    std::cout << "result: " << resp._result << "[" << resp._status << "]" << std::endl;
}

int main(int argc, char *argv[])
{
    srand((unsigned int)time(nullptr));

    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    std::unique_ptr<Socket> socket = std::make_unique<TCPSocket>();
    socket->BuildTcpClientMethod();

    InetAddr serveraddress(server_port, server_ip);
    bool n = socket->Connect(serveraddress); // 发起连接
    if (!n)                                  // 连接失败
    {
        std::cerr << "connect error: " << serveraddress.ToString() << std::endl;
        exit(2);
    }

    Protocol protocol(HandlerResponse); // 定义协议对象，注册处理应答的回调函数
    std::string inbuffer;

    while (true)
    {
        int cnt = 3;
        std::string outbuffer;
        while (cnt--)   // 多条请求
        {
            char number[5] = {'+', '-', '*', '/', '%'};
            // 0. 获取数据
            auto x = rand() % 10 + 1;
            auto y = rand() % 10 + 1;
            char oper = number[rand() % 4];
            // 1. 请求变量
            Request req(x, y, oper);
            // 2. 序列化
            std::string req_json;
            req.Serialize(&req_json);
            // 3. 封装报头
            std::string send_req_string = protocol.Packet(req_json); // 将来要发送的字符串

            outbuffer += send_req_string;
        }

        // 4. 发送
        socket->SendData(outbuffer);
        // 5. 接收计算结果
        socket->RecvData(&inbuffer);
        // 6. 解析应答结果
        protocol.ParseResponse(inbuffer);
    }

    // Protocol protocol(HandlerResponse);    // 定义协议对象，注册处理应答的回调函数
    // std::string inbuffer;

    // while(true)
    // {
    //     char number[5] = { '+', '-', '*', '/', '%' };
    //     // 0. 获取数据
    //     auto x = rand() % 10 + 1;
    //     auto y = rand() % 10 + 1;
    //     char oper = number[rand() % 4];
    //     // 1. 请求变量
    //     Request req(x, y, oper);
    //     // 2. 序列化
    //     std::string req_json;
    //     req.Serialize(&req_json);
    //     // 3. 封装报头
    //     std::string send_req_string = protocol.Packet(req_json);    // 将来要发送的字符串
    //     // 4. 发送
    //     socket->SendData(send_req_string);
    //     // 5. 接收计算结果
    //     socket->RecvData(&inbuffer);
    //     // 6. 解析应答结果
    //     protocol.ParseResponse(inbuffer);
    // }

    socket->CloseSockfd();

    return 0;
}