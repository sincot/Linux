#include "mystdio.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// 创建文件默认的权限
#define MODE 0666

// fopen 函数
myFILE* myfopen(const char* path, const char* mode)
{
  // 确认打开文件的文件描述符
  int fd = -1;
  // 文件打开的方式
  int flags = 0;

  // 以 r 的方式打开
  if(strcmp(mode, "r") == 0)
  {
    flags = O_RDONLY;
    fd = open(path, flags);
  }
  // 以 w 的方式打开
  else if(strcmp(mode, "w") == 0)
  {
    flags = O_WRONLY | O_CREAT | O_TRUNC;
    fd = open(path, flags, MODE);
  }
  // 以 a 的方式打开
  else if(strcmp(mode, "a") == 0)
  {
    flags = O_WRONLY | O_CREAT | O_APPEND;
    fd = open(path, flags, MODE);
  }
  else 
  {}

  // 判断是否打开成功
  if(fd < 0)
  {
    perror("open");
    return NULL;
  }

  // 打开文件成功
  // 注意到返回值是指针类型，一次需要创建一个 file 结构体对象
  myFILE* fp = (myFILE*)malloc(sizeof(myFILE));
  // 创建失败
  if(fp == NULL)
  {
    perror("malloc");
    return NULL;
  }

  // 将打开文件的信息记录到新建的 fp 对象中
  fp->fd = fd;
  fp->flags = flags;
  //fp->flush_mode = LINE_BUFFER;
  fp->flush_mode = FULL_BUFFER;
  fp->cap = SIZE;
  fp->pos = 0;

  return fp;
}

#define TRY_FLUSH 1
#define MUST_FLUSH 2

// fflush 函数
void myfflushcore(myFILE* fp, int flag)
{
  // 刷新缓冲区的前提是缓冲区中有数据
  if(fp->pos == 0)
  {
    return;
  }

  // 行刷新 — 这样有效减少了调用系统调用的次数
  // 行刷新的两种条件
  // 刷新方式为行刷新
  // 强制刷新
  if((fp->flush_mode & LINE_BUFFER) || (flag & MUST_FLUSH))
  {
    // 判断缓冲区可以刷新
    // 缓冲区的最后一个字符为\n，就可以行刷新了
    // 为强制刷新，就行刷新
    if((fp->outbuffer[fp->pos-1] == '\n') || (flag & MUST_FLUSH))
    {
      // 刷新是将数据从用户缓冲区写给OS,使用write函数
      // write 函数的函数原型:ssize_t write(int fd, const void *buf, size_t count)
      write(fp->fd, fp->outbuffer, fp->pos);
      // 缓冲区的数据到刷新到OS中了，清空缓冲区
      fp->pos = 0;
    }
  }
  // 全缓冲刷新
  else if((fp->flush_mode & FULL_BUFFER) || (flag & MUST_FLUSH))
  {
    // 判断缓冲区可以刷新
    // 可以自己设置全缓冲刷新的条件
    // 可以缓冲区的字符串长度等于缓冲区的三分之二时，就刷新
    // 可以缓冲区的字符串长度等于缓冲区时，就刷新
    // 可以缓冲区的字符串长度等于缓冲区的3/4时，就刷新
    if((fp->pos == fp->cap) || (flag & MUST_FLUSH))
    {
      write(fp->fd, fp->outbuffer, fp->pos);
      fp->pos = 0;
    }
  }
  // 无缓冲
  else if(fp->flush_mode & NON_BUFFER)
  {
    // 调用系统调用 write 
    write(fp->fd, fp->outbuffer, fp->pos);
  }
}

// myfflush 函数
void myfflush(myFILE* fp)
{
  myfflushcore(fp, MUST_FLUSH);
}

// fputs 函数
int myfputs(const char* s, myFILE* fp)
{
  // 字符串不能为空
  if(strlen(s) == 0)
  {
    return 0;
  }


  size_t len = strlen(s);   // 计算要写入缓冲区的字符串的长度
  const char* ptr = s;        // 字符串写入的位置
  while (len > 0) 
  {
    size_t space = fp->cap - fp->pos;   // 计算当前缓冲区还能容纳多少字节
    // 如果字符串的长度大于当前缓冲区能够容纳的字节，能容纳多少就写多少
    size_t write = (len > space) ? space : len;

    // 将 write 个字节从p拷贝到缓冲区的尾部
    memcpy(fp->outbuffer + fp->pos, ptr, write);
    // 更新缓冲区写入的位置
    fp->pos += write;
    // 移动指针ptr,跳过字符串以及写入的部分
    ptr += write;
    // 计算字符串写入缓冲区后，还剩多少字节
    len -= write;

    myfflushcore(fp, TRY_FLUSH);  // 尝试刷新
  }



  //// 将字符串s拷贝到fp指向的缓冲区中
  //memcpy(fp->outbuffer + fp->pos, s, strlen(s)); 

  //// 下次写的位置不能覆盖式的写
  //// 需要更新缓冲区被使用的大小
  //fp->pos += strlen(s);

  //// 判断是否满足刷新条件
  //// 调用 myfflush 函数
  //myfflushcore(fp, TRY_FLUSH);

  // fputs 函数的返回值是写入缓冲区的字符串长度
  return strlen(s);
}

// fclose 函数
void myfclose(myFILE* fp)
{
  // 强制刷新缓冲区
  myfflush(fp);

  // 关闭文件
  close(fp->fd);

  // 释放file对象
  free(fp);
}



