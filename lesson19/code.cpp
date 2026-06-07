#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

//#define WIFEXITED(status) (status&0x7F)  // 进程接收到的信号
//#define WEXITSTATUS(status) (status >> 8)&0xFF // 进程的退出码


// 一些函数
void Func1()
{
  std::cout << "Func1()" << std::endl;
}

void Func2()
{
  std::cout << "Func2()" << std::endl;
}
 
void Func3()
{
  std::cout << "Func3()" << std::endl;
}


// 函数指针
typedef void (*call_back)();

// 自定义错误码
enum
{
  right,
  USAGE_ERROR
};

// 子进程要执行的任务
void Task()
{
  int count = 5;
  while(count--)
  {
    printf("I am a child process, pid: %d, ppid: %d, count: %d\n", getpid(), getppid(), count);
    sleep(2);
  }
}

// 习惯规定：
// 输入: 参数使用 const &
// 输出: 参数使用 *
// 输入输出: 参数使用 &
void CreateChileProcess(int num, std::vector<pid_t>* vid, call_back cb)
{
  // 创建一批子进程
  for(int i = 0; i < num; i++)
  {
    pid_t id = fork();
    
    if(id == 0)
    {
      // 子进程
      cb();   // 子进程要执行的任务
      exit(0);
    }

    // vector容器存储子进程的pid值
    vid->push_back(id);
  }
}

void WaitChildProcess(const std::vector<pid_t>& vid)
{  
  // 等待子进程
  for(auto& pid : vid)
  { 
    // 父进程如何知道自己要等多少个子进程？
    // 使用 vector 容器,将每次创建的子进程的pid值存储起来
    // 父进程
    int status = 0;
    pid_t wid = waitpid(pid, &status, 0);
    if(wid > 0)
    {
      printf("子进程: %d 退出,wait_code: %d\n", wid, WEXITSTATUS(status));
      sleep(2);
    }
  }
}

int main(int argc, char* argv[])
{ 
  if(argc != 2)
  {
    // 指令的输入格式,要创建5个子进程
    std::cout << "Uasge: " << argv[0] << " process_num" << std::endl;
    exit(USAGE_ERROR);
  }

  // 得到的字符串，可是我们需要整数，怎么将字符串转化成整数，使用函数stoi
  int num = std::stoi(argv[1]); // 代表 process_num

  // 保存每次创建子进程的pid值
  std::vector<pid_t> vid;

  // 将创建子进程封装成函数
  CreateChileProcess(num, &vid, Task);
    
  // 父进程
  // 将父进程等待子进程封装成一个函数
  WaitChildProcess(vid);

  return right;
}






















  //printf("I am a process,pid = %d, ppid = %d\n", getpid(), getppid());

  //std::vector<call_back> tasks;
  //tasks.push_back(Func1);
  //tasks.push_back(Func2);
  //tasks.push_back(Func3);

  //pid_t id = fork();
  //if(id < 0)
  //{
  //  // 打印fork失败的原因
  //  perror("fork");
  //  exit(1);
  //}
  //else if(id == 0)
  //{
  //  // 子进程
  //  int cnt = 5;
  //  while(cnt--)
  //  {
  //    printf("I am a child process,pid = %d, ppid = %d\n", getpid(), getppid());
  //    sleep(1);
  //  }
  //}
  //else
  //{
  //  // 父进程
  //  while(1)
  //  {
  //      // 等待任意子进程 --- -1
  //      // 退出码为空,第三参数默认为0
  //      int status = 0;
  //      pid_t wid = waitpid(id, NULL, WNOHANG); // 非阻塞检测进程并回收
  //      // 子进程终止了
  //      if(wid > 0)
  //      {
  //        //printf("wait sucess, 退出的子进程是: %d, exit_code: %d, exit_signal: %d\n",\
  //               wid, (status >> 8)&0xFF, status&0x7F);
  //        
  //        //printf("wait sucess, 退出的子进程是: %d, exit_code: %d, exit_signal: %d\n",\
  //               wid, WEXITSTATUS(status), WIFEXITED(status));

  //        if(WIFEXITED(status))
  //        {
  //          printf("wait sucess,退出的子进程是: %d, exit_code: %d\n", wid, WEXITSTATUS(status));
  //        }
  //        else 
  //        {
  //          printf("子进程是异常退出的\n");
  //        }

  //        break;
  //      }
  //      // 子进程未终止
  //      else if(wid == 0)
  //      {
  //        printf("子进程仍未退出,父进程还需等待\n");
  //        sleep(2);
  //        // 等待期间执行其它任务
  //        for(auto& task : tasks)
  //        {
  //          task();
  //        }
  //      }
  //      else 
  //      {
  //        printf("waitpid fails, ret: %d", wid);
  //        perror("waitpid");
  //      }
  //  }
  //}




















  //printf("I am a process,pid = %d, ppid = %d\n", getpid(), getppid());

  //pid_t id = fork();
  //if(id < 0)
  //{
  //  // 打印fork失败的原因
  //  perror("fork");
  //  exit(1);
  //}
  //else if(id == 0)
  //{
  //  // 子进程
  //  int cnt = 5;
  //  while(cnt--)
  //  {
  //    printf("I am a child process,pid = %d, ppid = %d\n", getpid(), getppid());
  //    sleep(1);
  //  }
  //}
  //else
  //{
  //  // 父进程
  //  // 等待任意子进程 --- -1
  //  // 退出码为空,第三参数默认为0
  //  int status = 0;
  //  pid_t wid = waitpid(-1, NULL, 0);
  //  if(wid > 0)
  //  {
  //    //printf("wait sucess, 退出的子进程是: %d, exit_code: %d, exit_signal: %d\n",\
  //           wid, (status >> 8)&0xFF, status&0x7F);
  //    
  //    //printf("wait sucess, 退出的子进程是: %d, exit_code: %d, exit_signal: %d\n",\
  //           wid, WEXITSTATUS(status), WIFEXITED(status));

  //    if(WIFEXITED(status))
  //    {
  //      printf("wait sucess,退出的子进程是: %d, exit_code: %d\n", wid, WEXITSTATUS(status));
  //    }
  //    else 
  //    {
  //      printf("子进程是异常退出的\n");
  //    }
  //  }
  //  else
  //  {
  //    // 等待失败,waitpid返回-1
  //    printf("ret: %d\n", wid);
  //    perror("waitpid");
  //  }
  //}
  

  //printf("I am a process,pid = %d, ppid = %d\n", getpid(), getppid());

  //pid_t id = fork();
  //if(id < 0)
  //{
  //  // 打印fork失败的原因
  //  perror("fork");
  //  exit(1);
  //}
  //else if(id == 0)
  //{
  //  // 子进程
  //  int cnt = 5;
  //  while(cnt--)
  //  {
  //    printf("I am a child process,pid = %d, ppid = %d\n", getpid(), getppid());
  //    sleep(1);
  //  }
  //}
  //else
  //{
  //  // 父进程
  //  while(1)
  //  {
  //      // 等待任意子进程 --- -1
  //      // 退出码为空,第三参数默认为0
  //      int status = 0;
  //      pid_t wid = waitpid(id, NULL, WNOHANG); // 非阻塞检测进程并回收
  //      // 子进程终止了
  //      if(wid > 0)
  //      {
  //        //printf("wait sucess, 退出的子进程是: %d, exit_code: %d, exit_signal: %d\n",\
  //               wid, (status >> 8)&0xFF, status&0x7F);
  //        
  //        //printf("wait sucess, 退出的子进程是: %d, exit_code: %d, exit_signal: %d\n",\
  //               wid, WEXITSTATUS(status), WIFEXITED(status));

  //        if(WIFEXITED(status))
  //        {
  //          printf("wait sucess,退出的子进程是: %d, exit_code: %d\n", wid, WEXITSTATUS(status));
  //        }
  //        else 
  //        {
  //          printf("子进程是异常退出的\n");
  //        }

  //        break;
  //      }
  //      // 子进程未终止
  //      else if(wid == 0)
  //      {
  //        printf("子进程仍未退出,父进程还需等待\n");
  //        sleep(2);
  //      }
  //      else 
  //      {
  //        printf("waitpid fails, ret: %d", wid);
  //        perror("waitpid");
  //      }
  //  }
  //}

  //return 0;
//}
