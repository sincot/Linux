#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

// 全局变量
int g_val = 10;

int main()
{
  


  // 打印全局变量的值和地址
  //printf("g_val: %d, &g_val: %p\n", g_val, &g_val);

  //// 创建子进程
  //pid_t id = fork();

  //if(id == 0)
  //{
  //  while(1)
  //  {
  //    printf("子进程: pid = %d, ppid = %d, g_val: %d, &g_val: %p\n", getpid(), getppid(), g_val, &g_val);
  //    sleep(2);
  //    g_val++;
  //  }
  //}
  //else
  //{
  //  while(1)
  //  {
  //    printf("父进程: pid = %d, ppid = %d, g_val: %d, &g_val: %p\n", getpid(), getppid(), g_val, &g_val);
  //    sleep(2);
  //  }
  //}
  return 0;
}


