#include "Pipe.hpp"

int main()
{
    Fifo fileServer;
    fileServer.CreatePipe();      // 创建管道文件

    fileServer.OpenPipe(Read_Only);   // 以只读的方式打开

    std::string msg_read;    // 接收从管道读过来的数据
    while(1)        // 循环读取数据
    {
        int n = fileServer.ReceiveMessage(msg_read);    // 接收数据
        if(n > 0) { std::cout << "Client Send# " << msg_read << std::endl; }
        else { break; }
    }

    // 跳出循环后，关闭管道
    fileServer.DeletePipe();      // 删除管道文件

    return 0;
}
