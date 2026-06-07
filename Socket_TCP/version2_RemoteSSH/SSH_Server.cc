#include "TcpServer.hpp"    // 负责 IO 处理
#include "Command.hpp"      // 上层

#include <memory>

using namespace LOGMOUDLE;

void Usage(const std::string &process)
{
    std::cerr << "Usage\n\t";
    std::cerr << process << " local_port" << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();

    // 获取用户输入的服务端主机信息
    uint16_t server_port = std::stoi(argv[1]);
    std::unique_ptr<Command> command = std::make_unique<Command>();        // 命令处理的类对象
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(server_port);     // 网络通信的类对象

    // 初始化服务器
    tsvr->InitTcpServer();
    // 注册服务
    tsvr->Register([&command](std::string cmd)->std::string{
        return command->Excute(cmd);
    }, 
        [&command]()->std::string{
        return command->GetCommandString();     // 获取命令行字符串
    });
    // 运行服务器
    tsvr->StartTcpServer();
    
    return 0;
}