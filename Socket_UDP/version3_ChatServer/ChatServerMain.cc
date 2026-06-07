#include "UdpSever.hpp"     // 获取数据
#include "ThreadPool.hpp"   // 执行处理动作
#include "Route.hpp"        // 将数据包装成任务交给特定的人去处理
#include "Logger.hpp"

#include <memory>

using namespace LOGMOUDLE;
using namespace THREAD_POOL;

// 线程池要执行的任务
using task_t = std::function<void()>;

static void Usage(const std::string &process)
{
    // LOG(LogLevel::WARNING) << "Usage\n\t" << process << " local_ip local_port";
    LOG(LogLevel::WARNING) << "Usage\n\t" << process << " local_port";
}

// ./server_udp ip port
// ./ server port
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        LOG(LogLevel::WARNING) << "Usage use error";
        exit(Usage_ERR);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();      // 显示器策略


    // 2. 构建网络服务，处理 IO 问题
    // 用户传递了服务器的ip和port，直接赋值
    uint16_t server_port = std::stoi(argv[1]);

    // 路由模块
    Route rt;
    // 线程池模块
    auto thread_pool = ThreadPool<task_t>::LazySingle();
    // 网络模块
    UdpServer usvr(server_port);
    usvr.InitUdpServer();
    usvr.RegisterService([&rt](const InetAddr &addr){
        rt.CheckUser(addr);     // 检查客户端的地址
    }, 
    [&rt, thread_pool](int sockfd, std::string msg){
        // 将消息转发给所有用户
        auto t = std::bind(&Route::Broadcast, &rt, sockfd, msg);
        // 将任务交给线程池处理
        thread_pool->PushQueue(t);
    });         // 注册服务
    usvr.RunUdpServer();

    return 0;
}