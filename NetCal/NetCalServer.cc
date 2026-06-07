#include "TcpServer.hpp"
#include "Protocol.hpp"
#include "NetCalculator.hpp"

#include <string>
#include <memory>

// netcal_client 目标IP 目标主机端口号
static void Usage(const std::string &process)
{
    std::cout << "Usage:\n\t" << process << " server_port" << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    uint16_t port = std::stoi(argv[1]);
    // 1. 定义计算器
    std::unique_ptr<Calculator> cal = std::make_unique<Calculator>();

    // 2. 定义协议对象
    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>(
        [&cal](Request &req)->Response
        {
            return cal->Execute(req);
        }
    );

    // 3. 定义网络对象 —— 注册网络服务
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(
        [&protocol](std::string &inbuffer)->std::string{
            return protocol->ParseRequest(inbuffer);     // 解析报文
        }, port
    );

    tsvr->Loop();  // 启动服务器
    
    return 0;
}