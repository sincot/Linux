#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  //int i = 0;

  //scanf("%d", &i);

  //printf("i = %d\n", i);
  
  int i = 0;
  scanf("%d", &i);

  while(1)
  {
    printf("hello linux\n");
    sleep(2);
  }


  //while(1)
  //{
    //printf("我是一个进程, pid = %d\n", getpid());
    //sleep(2);
  //}

  //chdir("/home/zs");
  //FILE *pf = fopen("test.txt", "w");
  //fclose(pf);


  //printf("我是一个进程,pid = %d; ppid = %d\n", getpid(), getppid());
  //printf("我是一个父进程,pid = %d; ppid = %d\n", getpid(), getppid());
  //
  //pid_t id = fork();

  //// 分流
  //// id 小于 0，说明创建子进程失败
  //if(id < 0)
  //{
  //  return 1;
  //}
  //// id 等于 0，说明是子进程
  //else if(id == 0)
  //{ 
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
  //  printf("I am a process,pid = %d; ppid = %d\n", getpid(), getppid());
  //  sleep(2);
  //}

  //while(1)
  //{
  //  printf("我是一个进程,pid = %d; ppid = %d\n", getpid(), getppid());
  //  //printf("我是一个进程(fork),pid = %d; ppid = %d\n", getpid(), getppid());
  //  sleep(2);
  //}

  return 0;
}


