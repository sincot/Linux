#include "InetAddr.hpp"
#include "Mutex.hpp"
#include "Socket.hpp"

#include <memory>
#include <unistd.h>
#include <signal.h>
#include <functional>

using namespace SOCKET_MOUDLE;
using namespace LOGMOUDLE;

static const uint16_t gport = 8080;

using Handler_t = std::function<std::string(std::string &)>;

class TcpServer
{
public:
    TcpServer(Handler_t handler, uint16_t port = gport) : _port(port), _handler(handler)
    {
        _listensocket = std::make_unique<TCPSocket>();
        _listensocket->BuildTcpSocketMethod(_port);
    }

    void Loop() // 循环逻辑
    {
        signal(SIGCHLD, SIG_IGN);
        while (true)
        {
            InetAddr clientaddr;
            auto sockfd = _listensocket->AcceptSocket(clientaddr); // 获取连接
            if (sockfd == nullptr)
                continue; // 为空，继续获取
            LOG(LogLevel::DEBUG) << "get a new link, socket address: " << clientaddr.ToString()
                                 << " socket: " << sockfd->GetSockfd();

            if (fork() == 0)
            {
                // 让子进程读数据和写数据
                ServiceIO(sockfd, clientaddr);
                sockfd->CloseSockfd();

                exit(0);
            }
        }
    }

    ~TcpServer() {}

private:
    void ServiceIO(std::shared_ptr<Socket> sockfd, InetAddr &clientaddr)
    {
        // 短服务
        std::string inbuffer, outbuffer;

        outbuffer.clear();

        int n = sockfd->RecvData(&inbuffer);    // 读取到完整的 HTTP 请求
        if (n <= 0)
        {
            LOG(LogLevel::WARNING) << "recv: client quit, " << clientaddr.ToString();
        }

        LOG(LogLevel::DEBUG) << "inbuffer:\n" << inbuffer;

        if (_handler)
        {
            outbuffer = _handler(inbuffer);
        } // 处理读到的数据

        if (outbuffer.empty())
            return;

        LOG(LogLevel::DEBUG) << "outbuffer:\n" << outbuffer;

        n = sockfd->SendData(outbuffer);
        if (n < 0)
        {
            LOG(LogLevel::WARNING) << "send: client quit, " << clientaddr.ToString();
        }
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensocket;
    Handler_t _handler;
};