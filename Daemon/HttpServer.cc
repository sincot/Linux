#include "TcpServer.hpp"
#include "HTTP.hpp"
#include "Socket.hpp"
#include "Daemon.hpp"

#include <string>
#include <memory>

// netcal_client 目标IP 目标主机端口号
static void Usage(const std::string &process)
{
    std::cout << "Usage:\n\t" << process << " server_port" << std::endl;
}

// 所提供的服务函数
// GET /exec HTTP/1.0
// GET /Login HTTP/1.0
void Login(HttpRequest &req, HttpResponse &resp)
{
    // 登录逻辑
    std::cout << "Login service" << std::endl;
    req.DebugPrint();  // 打印 HTTP 请求对象的内容

    std::string body = req.GetBody();    // 获取请求正文
    std::cout << "Request body: " << body << std::endl;

    resp.SetCode(200);   // 设置状态码和状态描述
    // 设置响应报头 Content-Length，指定响应正文的长度
    resp.AddHeader("Content-Length", std::to_string(resp.GetBodyLength()));
    // 设置响应报头 Content-Type，指定响应正文的类型
    resp.AddHeader("Content-Type", MineType::SuffixToMineType(req.GetSuffix()));
    resp.SetBody("Login successful!");    // 设置响应正文
}

void Register(HttpRequest &req, HttpResponse &resp)
{
    // 注册逻辑
}

void Serch(HttpRequest &req, HttpResponse &resp)
{
    // 搜索逻辑
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    // 让日志向文件里写
    ENABLE_FILE_LOG_STRATEGY();
    // 获取用户传递过来的服务器端口号
    uint16_t port = std::stoi(argv[1]);

    // 不想再打印之前的调试信息了，printf 和 cout 应该怎么办？它们又不能向文件中写
    // 解决方案：
    // 1. 改代码，将 printf 和 cout 删掉 —— 不推荐，打印信息需要保留，方便调试
    // 2. 将 printf 和 cout 的输出重定向到 /dev/null 文件中
    // Daemon(1, 1);   // 1. 将进程变成守护进程，使用自定义的
    daemon(0, 0);    // 1. 将进程变成守护进程，使用系统提供的系统调用

    // 2. 定义 HTTP 协议
    std::unique_ptr<HttpProtocol> protocol = std::make_unique<HttpProtocol>();
    protocol->RegisterService("/login", Login);          // 注册登录服务函数
    protocol->RegisterService("/register", Register);    // 注册注册服务函数
    protocol->RegisterService("/search", Serch);         // 注册搜索服务函数

    // 3. 定义网络对象 —— 注册网络服务
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(
        [&protocol](std::string &inbuffer)->std::string{
            return protocol->HandlerHttpRequest(inbuffer);
        }, port
    );

    tsvr->Loop();  // 启动服务器
    
    return 0;
}