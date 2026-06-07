#include "EchoSever.hpp"
#include "Logger.hpp"
#include "Dict.hpp"

#include <memory>

using namespace LOGMOUDLE;

static void Usage(const std::string &process)
{
    // LOG(LogLevel::WARNING) << "Usage\n\t" << process << " local_ip local_port";
    LOG(LogLevel::WARNING) << "Usage\n\t" << process << " local_port";
}

// ./server_udp ip port
// ./ server port
int main(int argc, char *argv[])
{
    // if(argc != 3)
    // {
    //     Usage(argv[0]);
    //     LOG(LogLevel::WARNING) << "Usage use error";
    //     exit(Usage_ERR);
    // }

    if(argc != 2)
    {
        Usage(argv[0]);
        LOG(LogLevel::WARNING) << "Usage use error";
        exit(Usage_ERR);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();      // 显示器策略

    // 1. 实例化字典对象
    Dict dict;

    // 2. 构建网络服务，处理 IO 问题
    // 用户传递了服务器的ip和port，直接赋值
    // std::string server_ip = argv[1];
    // uint16_t server_port = std::stoi(argv[2]);
    uint16_t server_port = std::stoi(argv[1]);

    // 3. 绑定上下层
    // 实现一个智能指针,使用用户传递的 port
    // std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(server_ip, server_porrt);
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>([&dict](std::string word)->std::string{
        return dict.Translate(word);
    }, server_port);

    // 初始化服务端
    usvr->InitUdpServer();

    // 启动服务器/服务端工作
    usvr->RunUdpServer();

    return 0;
}