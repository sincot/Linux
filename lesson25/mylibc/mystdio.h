#pragma once 

#include <stdio.h>

//#define SIZE 1024   // 缓冲区的大小
#define SIZE 128

// 缓冲区的刷新方式
#define NON_BUFFER 1    // 0001
#define LINE_BUFFER 2   // 0010
#define FULL_BUFFER 4   // 0100

// file 结构体
typedef struct _myFILE
{
  int fd;   // 文件描述符
  int flags;    // 文件的打开方式
  int flush_mode;   // 缓冲区的刷新方式
  char outbuffer[SIZE];   // 输出缓冲区
  int cap;    // 缓冲区的总容量
  int pos;    // 缓冲区已经被使用多少
}myFILE;

// fopen 函数
myFILE* myfopen(const char* path, const char* mode);

// fputs 函数
int myfputs(const char* s, myFILE* fp);

// fflush 函数
void myfflush(myFILE* fp);

// fclose 函数
void myfclose(myFILE* fp);
