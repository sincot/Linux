#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

//int main()
//{
//  return 100;
//}

void Func()
{
  printf("hello linux\n");
  _exit(10);
}

int main()
{
  //printf("hello linux");
  //sleep(2);
  //_exit(100);

  //printf("I am a process, pid = %d, ppid = %d\n", getpid(), getppid());
  //Func();
  //exit(100);
  //FILE* pf = fopen("test.txt", "r");
  //if(pf == NULL)
  //{
  //  printf("%d: %s\n", errno, strerror(errno));
  //}

  //while(1)
  //{
  //  printf("I am a process!\n");
  //  sleep(2);
  //}
  
  //pid_t id = fork();
  //if(id == 0) 
  //{
  //  int count = 5;
  //  while(count--)
  //  {
  //    printf("I am 子进程: pid = %d, ppid = %d\n", getpid(), getppid());
  //    sleep(2);
  //  }

  //  // 直接让子进程终止，进入僵尸状态
  //  exit(0);
  //}
  //else if(id > 0)
  //{
  //  //while(1)
  //  //{
  //  //  printf("I am 父进程: pid = %d, ppid = %d\n", getpid(), getppid());
  //  //  sleep(2);
  //  //}
  //  
  //  // 子进程需要经过10s后才退出
  //  // 为了清晰的看到wait确实解决子进程僵尸问题，可以让父进程休眠15s
  //  sleep(15);
  //  // 回收子进程，等待子进程的僵尸状态
  //  pid_t rid = wait(NULL);
  //  // 若rid等于子进程的id,说明父进程等待成功了
  //  if(rid == id)
  //  {
  //    printf("pid = %d 等待成功!\n", getpid());
  //  }
  //  // wait成功之后，让父进程休眠5s，在退出
  //  sleep(5);
  //  exit(0);
  //}
  //else 
  //{}
 
   
  pid_t id = fork();
  if(id == 0) 
  {
    int count = 5;
    while(count--)
    {
      printf("I am 子进程: pid = %d, ppid = %d\n", getpid(), getppid());
      sleep(2);
    }

    // 直接让子进程终止，进入僵尸状态
    exit(1);
  }
  else if(id > 0)
  {
    //while(1)
    //{
    //  printf("I am 父进程: pid = %d, ppid = %d\n", getpid(), getppid());
    //  sleep(2);
    //}
    
    // 子进程需要经过10s后才退出
    // 为了清晰的看到wait确实解决子进程僵尸问题，可以让父进程休眠15s
    sleep(15);
    // 回收子进程，等待子进程的僵尸状态
    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    // 若rid等于子进程的id,说明父进程等待成功了
    if(rid == id)
    {
      printf("pid = %d 等待成功! status = %d\n", getpid(), status);
    }
    // wait成功之后，让父进程休眠5s，在退出
    sleep(5);
  } 
  else 
  {}
  //pid_t id = fork();
  //if(id == 0)
  //{
  //  while(1)
  //  {
  //    printf("I am 子进程: pid = %d, ppid = %d\n", getpid(), getppid());
  //    sleep(2);
  //  }

  //  // 直接让子进程终止，进入僵尸状态
  //  exit(1);
  //}
  //else if(id > 0)
  //{
  //  //while(1)
  //  //{
  //  //  printf("I am 父进程: pid = %d, ppid = %d\n", getpid(), getppid());
  //  //  sleep(2);
  //  //}
  //  
  //  // 子进程需要经过10s后才退出
  //  // 为了清晰的看到wait确实解决子进程僵尸问题，可以让父进程休眠20s
  //  // 回收子进程，等待子进程的僵尸状态
  //  int status = 0;
  //  pid_t rid = waitpid(id, &status, 0);
  //  // 若rid等于子进程的id,说明父进程等待成功了
  //  if(rid == id)
  //  {
  //    // status >> 8 — 将 status 右移8位，让退出码移动到低8位
  //    // &0xFF ——取低8位，得到实际的退出码
  //    int exit_code = (status >> 8)&0xFF;
  //    // 取低7位，获取的是导致进程终止的信号编号 
  //    int exit_signal = status&0x7F;
  //    printf("pid = %d 等待成功! status: %d, eixt_code: %d, exit_signal: %d\n", getpid(), status, exit_code, exit_signal);
  //  }
  //  // wait成功之后，让父进程休眠5s，在退出
  //}
  //else 
  //{}

  //for(int i = 0; i < 140; i++)
  //{
  //  printf("%d->%s\n", i, strerror(i));
  //}
  
  //printf("I am a process, pid = %d, ppid = %d\n", getpid(), getppid());
  //exit(100);

  return 0;
}


