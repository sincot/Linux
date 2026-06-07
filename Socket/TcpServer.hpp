#include "Logger.hpp"
#include "InetAddr.hpp"
#include "Socket.hpp"

#include <memory>

using namespace LOGMOUDLE;
using namespace SOCKET_MOUDLE;

static uint16_t gport = 8080;

// 底层网络通信
class TcpServer
{
public:
    TcpServer(uint16_t port = gport) 
        :_port(port),  _listensocket(std::make_unique<TCPSocket>())
    {
        _listensocket->BuildTcpSocketMethod(port);  // 使用创建 listensocket 的模板方法
    }

    void Loop()     // 执行循环逻辑
    {
        while(true)
        {
            
        }
    }


    ~TcpServer() {}

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensocket;
};