#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>

// 将标准输入 0 设置成非阻塞 —— 调用 fcntl 函数
void set_nonblocking(int fd)
{
    // 调用 fcntl 函数，函数原型：int fcntl(int fd, int op, ... /* arg */ )
    int flags = fcntl(fd, F_GETFL, 0);    // 获取文件描述符的当前状态标志
    if(flags < 0)
    {
        std::cerr << "Error getting file flags." << std::endl;
        return;    // 获取文件标志失败，退出函数    
    }
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);    // 将文件描述符设置为非阻塞模式
}


int main()
{
    char inbuffer[1024];
    // 将标准输入0设置为非阻塞模式
    set_nonblocking(0);

    while(true)
    {
        ssize_t n = read(0, inbuffer, sizeof(inbuffer));    // 调用read函数，从标准输入读取数据
        if(n > 0)           // 读取成功
        {
            inbuffer[n] = '\0';    // 将读取到的数据转换为字符串
            std::cout << "read: " << inbuffer << std::endl;
        }
        else if(n == 0)     // 读到文件结尾
        {
            std::cout << "End of file reached." << std::endl;
            // break;    // 读取到文件末尾，退出循环
        }
        else                // 读取发生错误
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)    // 没有数据可读，继续循环
            {
                // 错误码为 EAGAIN，表示数据未就绪
                std::cerr << "No data available to read, errno: " << errno << std::endl;    
                std::cerr << strerror(errno) << std::endl;
                continue;    // 没有数据可读，继续循环
            }
            else if(errno == EINTR)    // 读取被信号中断，继续循环
            {
                std::cerr << "Read interrupted by signal, errno: " << errno << std::endl;
                std::cerr << strerror(errno) << std::endl;
                continue;    // 读取被信号中断，继续循环
            }
            else    // 读取失败
            {
                std::cerr << "Error reading from standard input, errno: " << errno << std::endl;
                std::cerr << strerror(errno) << std::endl;
                break;    // 读取发生错误，退出循环
            }
            // 读取错误
            // std::cerr << "Error reading from standard input, errno: " << errno << std::endl;
            // std::cerr << strerror(errno) << std::endl;
            // break;    // 读取发生错误，退出循环
        }

        sleep(1);
    }

    return 0;
}