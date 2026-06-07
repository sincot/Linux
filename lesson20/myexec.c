#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
  














  //printf("I am a process,pid: %d\n", getpid());

  //pid_t id = fork();
  //if(id == 0)
  //{
  //  // 执行另一个程序的代码
  //  execl("/usr/bin/ls", "-a", "-l", NULL);
  //  exit(0);
  //}

  //// 父进程
  //wait(NULL);
  //printf("正在执行自己的代码!\n");
  //printf("正在执行自己的代码!\n");
  //printf("正在执行自己的代码!\n");
  //printf("正在执行自己的代码!\n");
  //printf("正在执行自己的代码!\n");



  //printf("I am a process,pid: %d\n", getpid());

  //pid_t id = fork();
  //if(id == 0)
  //{
  //  // 执行另一个程序的代码
  //  execl("/usr/bin/ls", "ls", "-a", "-l", NULL);
  //  exit(1);
  //}

  //int status = 0;
  //pid_t wid = waitpid(id, &status, 0);
  //if(wid > 0)
  //{
  //  printf("wait success, exit_code: %d\n", WEXITSTATUS(status));
  //}


  //printf("I am a process,pid: %d\n", getpid());

  //pid_t id = fork();
  //if(id == 0)
  //{
  //  // 执行另一个程序的代码
  //  execlp("ls", "ls", "-a", "-l", NULL);
  //  exit(1);
  //}

  //int status = 0;
  //pid_t wid = waitpid(id, &status, 0);
  //if(wid > 0)
  //{
  //  printf("wait success, exit_code: %d\n", WEXITSTATUS(status));
  //}


  //printf("I am a process,pid: %d\n", getpid());

  //pid_t id = fork();
  //if(id == 0)
  //{
  //  // 执行另一个程序的代码
  //  char* argv[] = { "ls", "-a", "-l", NULL };
  //  execv("/usr/bin/ls", argv);
  //  exit(1);
  //}

  //int status = 0;
  //pid_t wid = waitpid(id, &status, 0);
  //if(wid > 0)
  //{
  //  printf("wait success, exit_code: %d\n", WEXITSTATUS(status));
  //}



  //printf("I am a process,pid: %d\n", getpid());

  //pid_t id = fork();
  //if(id == 0)
  //{
  //  // 执行另一个程序的代码
  //  char* argv[] = { "ls", "-a", "-l", NULL };
  //  execvp(argv[0], argv);
  //  exit(1);
  //}

  //int status = 0;
  //pid_t wid = waitpid(id, &status, 0);
  //if(wid > 0)
  //{
  //  printf("wait success, exit_code: %d\n", WEXITSTATUS(status));
  //}



  //printf("I am a process,pid: %d\n", getpid());

  //pid_t id = fork();
  //if(id == 0)
  //{
  //  // 执行另一个程序的代码
  //  execl("./test", "test", NULL);
  //  exit(1);
  //}

  //int status = 0;
  //pid_t wid = waitpid(id, &status, 0);
  //if(wid > 0)
  //{
  //  printf("wait success, exit_code: %d\n", WEXITSTATUS(status));
  //}


  printf("I am a process,pid: %d\n", getpid());

  pid_t id = fork();
  if(id == 0)
  {
    // 执行另一个程序的代码
    char* argv[] = { "./test", "-a", "-b", NULL };
    char* env[] = { "PATH=/home/zs/Linux/lesson19", NULL };
    extern char** environ;
    //execvpe("./test", argv, env); // 覆盖式的使用全新的环境变量表
    execvpe("./test", argv, environ);   // 使用父进程的环境变量表
    //execvpe("test", argv, env);
    //execvpe("test", argv, environ);
    exit(1);
  }

  int status = 0;
  pid_t wid = waitpid(id, &status, 0);
  if(wid > 0)
  {
    printf("wait success, exit_code: %d\n", WEXITSTATUS(status));
  }






  //printf("I am a process,pid: %d\n", getpid());

  //// 执行另一个进程
  //execl("/usr/bin/ls", "-a", "-l", NULL);  //execl是一个程序替换函数

  //printf("正在执行自己的程序!\n");
  //printf("正在执行自己的程序!\n");
  //printf("正在执行自己的程序!\n");
  //printf("正在执行自己的程序!\n");
  //printf("正在执行自己的程序!\n");

  return 0;
}


