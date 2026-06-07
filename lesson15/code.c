#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

//int main()
//{
//  while(1)
//  {
//    printf("我是一个进程,pid = %d\n", getpid());
//    sleep(2);
//  }
//  return 0;
//}


int main()
{
  //while(1)
  //{
  //  printf("我是一个进程,pid = %d;我的父进程,ppid = %d\n", getpid(), getppid());
  //  sleep(2);
  //}
  
  chdir("/home/zs");
  FILE* pf = fopen("test.txt", "w");
  fclose(pf);

  printf("我是一个进程,pid = %d; ppid = %d\n", getpid(), getppid());
  fork();
  printf("我是一个进程(fork),pid = %d; ppid = %d\n", getpid(), getppid());

  return 0;
}
