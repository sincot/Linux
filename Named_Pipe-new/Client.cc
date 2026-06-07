#include "Pipe.hpp"

int main()
{
    Fifo fileClient;
    fileClient.OpenPipe(Write_Only);    // 以只写的方式打开

    while(1)    // 不断的写入数据
    {
        std::cout << "Please Enter$ ";
        std::string msg_write;
        std::getline(std::cin, msg_write);       // 用户输入数据
        fileClient.SendMessage(msg_write);       // 向管道写数据
    }

    return 0;
}

