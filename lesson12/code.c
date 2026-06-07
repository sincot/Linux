#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  while(1)
  {
    printf("I am a process, pid = %d\n", getpid());
    sleep(2);
  }

  //pid_t id = fork();
  //if(id == 0)
  //{
  //  //子进程
  //  while(1)
  //  {
  //    printf("I am 子进程,pid = %d, ppid = %d\n", getpid(), getppid());
  //    sleep(2);
  //  }
  //}
  //else
  //{
  //  //父进程
  //  int count = 5; // 限制父进程的执行次数
  //  while(count)
  //  {
  //    printf("I am 父进程,pid = %d, ppid = %d, count = %d\n", getpid(), getppid(), count);
  //    sleep(2);
  //    count--;
  //  }
  //}

  //printf("I am 父进程:%d\n", getpid());
  //sleep(2);

  //pid_t id = fork();
  //if(id == 0)
  //{
  //  // 子进程
  //  while(1)
  //  {
  //    printf("I am 子进程(fork):%d，我的父进程:%d\n", getpid(), getppid());
  //    sleep(2);
  //  }
  //}
  //else
  //{
  //  // 父进程
  //  while(1)
  //  {
  //    printf("I am 父进程(fork):%d", getpid());
  //    sleep(2);
  //  }
  //}

  //int i = 10;
  //scanf("%d", &i);
  //while(1)
  //{
  //  printf("hello linux\n");
  //  sleep(2);
  //}

  return 0;
}

