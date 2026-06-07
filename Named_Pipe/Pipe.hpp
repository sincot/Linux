#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


// 保证 Client 和 Server 访问的是同一个路径下的同一个文件
const std::string gfilepath = "./fifo";

// 自定义两个标志位，以读方式打开和以写方式打开
#define Read_Only 0
#define Write_Only 1

// 错误码表
enum 
{
    NO_PROBLEM = 0,     // 没有问题
    CRETEPIPE_ERROR,    // 创建管道失败
    OPENRIPE_ERROR,     // 打开管道失败
    WRITEPIPE_ERROR,    // 读取管道失败
};

// 管道类
class Fifo
{
public:
    Fifo(const std::string &filepath = gfilepath) 
        : _filepath(filepath), _mode(0666), _fd(-1)
    {}

    // 1. 创建管道
    void CreatePipe()
    {
        // 如果管道存在，就不要再执行下去
        if(PipeIsExit() == true) { return; }
        errno = 0;  // 为了避免 PipeIsExit 函数的退出码影响本函数，重置 errno
        // 调用 mkfifo 函数：int mkfifo(const char *pathname, mode_t mode)
        int n = mkfifo(_filepath.c_str(), _mode);
        if(n < 0)   // 管道创建失败
        {
            // strerror 打印错误码信息，错误码保存在全局变量 errno 中
            std::cerr << "mkfifo error: " << strerror(errno) <<  " errno: " << errno << std::endl;
            exit(CRETEPIPE_ERROR);
        }
        // 创建成功
        std::cerr << "mkfifo success: " << strerror(errno) <<  " errno: " << errno << std::endl; 
    }

    // 2. 删除管道
    void DeletePipe()
    {
        // 如果管道不存在，就不要再执行下去了
        if(PipeIsExit() == false) { return; }
        int n = unlink(_filepath.c_str());
        printf("删除 %s 成功\n", _filepath.c_str()); 
    }

    // 3. 打开管道
    void OpenPipe(int mode)
    {
        // int open(const char *pathname, int flags, ... /* mode_t mode */ )
        if(mode == Read_Only)       // 以读方式打开
        {
            // 以只读的方式打开
            _fd = open(_filepath.c_str(), O_RDONLY);
        }
        else if(mode == Write_Only)    // 以写方式打开
        {
            _fd = open(_filepath.c_str(), O_WRONLY);
        }
        else {}
        
        // 如果 _fd 仍然为 -1,说明打开失败了
        if(_fd < 0)
        {
            std::cerr << "open error: " << strerror(errno) << "errno" << errno << std::endl;
            exit(OPENRIPE_ERROR);
        }
    }

    // 发送数据
    void SendMessage(const std::string &msgin)
    {
        // ssize_t write(int fd, const void buf[.count], size_t count);
        ssize_t n = write(_fd, msgin.c_str(), msgin.length());
        (void)n;
    }

    // 接收数据
    int ReceiveMessage(std::string &msgout)
    {
        char outbuffer[512];
        // ssize_t read(int fd, void buf[.count], size_t count);
        ssize_t n = read(_fd, outbuffer, sizeof(outbuffer)-1);  // 保留 \0 的位置
        if(n > 0)   // 读写成功
        {
            outbuffer[n] = 0;  // 手动加上 \0
            msgout = outbuffer;    // 获取读到的数据
            return n;
        }
        else if(n == 0)     // 读到缓冲区的结尾
        {
            return 0;
        }
        else                // 读写失败
        {
            return WRITEPIPE_ERROR;
        }
    }

    ~Fifo() 
    {}

private:
    bool PipeIsExit()
    {
        // int stat(const char *restrict pathname, struct stat *restrict statbuf)
        struct stat st;
        int n = stat(_filepath.c_str(), &st);
        // 获取文件属性成功，文件存在，返回true
        if(n == 0) 
        { 
            std::cout << "file exits" << std::endl;
            return true; 
        }
        // 获取文件属性失败，文件不存在，返回 false
        else 
        { 
            std::cout << "file not exits" << " errno: " << errno << std::endl;
            return false; 
        }
    }

private:
    std::string _filepath;
    mode_t _mode;
    int _fd;
};

