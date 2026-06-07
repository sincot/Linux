#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  //printf("我是一个进程,pid = %d; ppid = %d\n", getpid(), getppid());
  //fork();
  //printf("我是一个进程(fork),pid = %d; ppid = %d\n", getpid(), getppid());

  //while(1)
  //{
  //  printf("I am a process,pid = %d; ppid = %d\n", getpid(), getppid());
  //  sleep(2);
  //}
  
  //printf("我是一个父进程,pid = %d; ppid = %d\n", getpid(), getppid());

  //pid_t id = fork();

  //// 分流
  //// id 小于 0，说明创建子进程失败
  //if(id < 0)
  //{
  //  return 1;
  //}
  //// id 大于等于 0，说明创建子进程成功
  //else if(id == 0)
  //{
  //  // id 等于 0，说明是子进程
  //  while(1)
  //  {
  //    printf("I am 子进程(fork),pid = %d; ppid = %d\n", getpid(), getppid());
  //    sleep(2);
  //  }
  //}
  //else
  //{
  //  // id 大于 0，说明是父进程
  //  while(1)
  //  {
  //    printf("I am 父进程(fork),pid = %d; ppid = %d\n", getpid(), getppid());
  //    sleep(2);
  //  }
  //}

  //printf("我是一个父进程,pid = %d; ppid = %d\n", getpid(), getppid());

  //pid_t id = fork();

  //// 分流
  //// id 小于 0，说明创建子进程失败
  //if(id < 0)
  //{
  //  return 1;
  //}
  //// id 大于等于 0，说明创建子进程成功
  //else if(id == 0)
  //{
  //  // id 等于 0，说明是子进程
  //  while(1)
  //  {
  //    printf("I am 子进程(fork),pid = %d; ppid = %d; id = %d\n", getpid(), getppid(), id);
  //    sleep(2);
  //  }
  //}
  //else
  //{
  //  // id 大于 0，说明是父进程
  //  while(1)
  //  {
  //    printf("I am 父进程(fork),pid = %d; ppid = %d; id = %d\n", getpid(), getppid(), id);
  //    sleep(2);
  //  }
  //}

  //while(1)
  //{
  //  printf("我是一个进程,pid = %d\n", getpid());
  //}
  
  //int i = 0;
  //scanf("%d", &i);
  //printf("i = %d\n", i);

  //while(1)
  //{
  //  sleep(2);
  //}

  //while(1)
  //{
  //  printf("hello linux\n");
  //  sleep(2);
  //}

  pid_t id = fork();

  if(id == 0)
  {
    // 子进程
    while(1)
    {
      printf("I am 子进程,pid = %d; ppid = %d\n", getpid(), getppid());
      sleep(2);
    }
  }
  else
  {
    // 父进程
    int count = 5; // 限制父进程的执行次数
    while(count--)
    {
      printf("I am 父进程,pid = %d; ppid = %d; count = %d\n", getpid(), getppid(), count);
      sleep(2);
    }
  }

  return 0;
}
