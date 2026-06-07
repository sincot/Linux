// #include "TcpServer.hpp"
#include "Reactor.hpp"
#include "Logger.hpp"
#include "Listener.hpp"

#include <iostream>
#include <unistd.h>
#include <memory>

using namespace LOGMOUDLE;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usge: " << argv[0] << " port" << std::endl;
        return 1;
    }

    ENABLE_CONSOLE_LOG_STRATEGY();  // 显示器打印

    uint16_t port = std::stoi(argv[1]);

    // 创建 Listener 对象
    std::shared_ptr<BaseConnection> listener = std::make_shared<Listener>(port);

    // 创建 TcpServer 对象
    // std::unique_ptr<TcpServer> tcpserver = std::make_unique<TcpServer>();
    // 创建 Reactor 对象
    std::unique_ptr<Reactor> reactor = std::make_unique<Reactor>();


    // // 将连接添加到 TcpServer 中
    // tcpserver->AddConnection(listener);

    // 将连接添加到 TcpServer 中
    reactor->AddConnection(listener);

    // 事件派发
    // tcpserver->TcpServerRun();

    // 事件派发
    reactor->TcpServerRun();

    return 0;
}

// 逻辑：
// 循环获取就绪的事件，调用 Dispatcher 函数
// 在此之前，已经将 listensocket 设置到内核中
// Dispatcher 函数中包含了 recver/sender/excepter函数，通过调用它们从 Reactor 模块到达 Listener 模块
// 