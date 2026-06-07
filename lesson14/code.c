#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// 未初始化的全局变量
int g_unval;
// 已初始化的全局变量
int g_val = 100;

int main(int argc, char *argv[], char *env[])
{
 // 打印str,打印的就是字符串的地址
 const char *str = "hello linux";
 printf("code addr: %p\n", main);               // 正文代码的地址
 printf("read only string addr: %p\n", str);    // 字符串常量区的地址
 printf("init global addr: %p\n", &g_val);      // 已初始化全局变量的地址
 printf("uninit global addr: %p\n", &g_unval);  // 未初始化全局变量的地址

 static int test1 = 10;                        // 静态成员变量
 int test2 = 10;
 // heap_mem 到 heap_mem3 的地址是连续的
 char *heap_mem = (char*)malloc(10);            
 char *heap_mem1 = (char*)malloc(10);
 char *heap_mem2 = (char*)malloc(10);
 char *heap_mem3 = (char*)malloc(10);
 
 // 打印堆区的地址,打印的是指针变量本身,而不是&指针变量
 printf("heap addr: %p\n", heap_mem); //heap_mem(0), &heap_mem(1)
 printf("heap addr: %p\n", heap_mem1); //heap_mem(0), &heap_mem(1)
 printf("heap addr: %p\n", heap_mem2); //heap_mem(0), &heap_mem(1)
 printf("heap addr: %p\n", heap_mem3); //heap_mem(0), &heap_mem(1)
 
 // 打印栈区的地址,打印的是指针变量的地址,即&指针变量
 printf("test static addr: %p\n", &test1); //heap_mem(0), &heap_mem(1)
 printf("test addr: %p\n", &test2);
 printf("stack addr: %p\n", &heap_mem); //heap_mem(0), &heap_mem(1)
 printf("stack addr: %p\n", &heap_mem1); //heap_mem(0), &heap_mem(1)
 printf("stack addr: %p\n", &heap_mem2); //heap_mem(0), &heap_mem(1)
 printf("stack addr: %p\n", &heap_mem3); //heap_mem(0), &heap_mem(1)

 int i = 0;
 for(;i < argc; i++)
 {
  printf("argv[%d]: %p\n", i, argv[i]);
 }
 for(; env[i]; i++)
 {
  printf("env[%d]: %p\n", i, env[i]);
 }

 return 0;
}


