#include "EpollServer.hpp"

#include <iostream>
#include <memory>
#include <unistd.h>


int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    ENABLE_CONSOLE_LOG_STRATEGY();  // 启用控制台日志策略


    uint16_t port = std::stoi(argv[1]);     // 获取端口号
    std::unique_ptr<EpollServer> server = std::make_unique<EpollServer>(port);

    server->EpollRun();      // 启动服务器

    return 0;
}