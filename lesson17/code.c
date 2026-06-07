#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

//int main()
//{
//  while(1)
//  {
//    printf("I am a process, pid = %d\n", getpid());
//    sleep(2);
//  }
//
//  return 0;
//}


//int main(int argc, char* argv[])
//{
//  printf("argc = %d\n", argc);
//
//  int i = 0;
//  for(; i < argc; i++)
//  {
//    printf("argv[%d]->%s\n", i, argv[i]);
//  }
//
//  return 0;
//} 


//int main(int argc, char* argv[])
//{
//  if(argc != 2)
//  {
//    printf("该命令使用错误,你应该这样使用: %s-a|-b|-c|-d\n", argv[0]);
//    return 1;
//  }
//
//  if(strcmp(argv[1], "-a") == 0)
//  {
//    printf("现在执行的是第一种功能\n");
//  }
//  else if(strcmp(argv[1], "-b") == 0)
//  {
//    printf("现在执行的是第二种功能\n");
//  }
//  else if(strcmp(argv[1], "-c") == 0)
//  {
//    printf("现在执行的是第三种功能\n");
//  }
//  else
//  {
//    printf("现在执行的是该命令的默认功能\n");
//  }
//  
//  int i = 0;
//  for(; i <= argc; i++)
//  {
//    printf("argv[%d]->%s\n", i, argv[i]);
//  }
//
//  printf("这是用户自己实现的一个命令!\n");
//
//  return 0;
//}


//int main(int argc, char* argv[], char* env[])
//{
//  int i = 0;
//  for(; env[i], i++)
//  {
//    printf("env[%d]->%s\n", i, env[i]);
//  }
//
//  return 0;
//}    

//int main()
//{
//  extern char** environ;
//  int i = 0;
//  for(; environ[i]; i++)
//  {
//    printf("environ[%d]->%s\n", i, environ[i]);
//  }
//
//  return 0;
//}

#include <stdlib.h>

//int main()
//{
//  char* whoami = getenv("USER");
//  if(whoami == NULL)
//  {
//    printf("执行失败,不认识该用户\n");
//  }
//  else if(strcmp(whoami, "root") == 0)
//  {
//    printf("执行失败,非目标用户\n");
//  }
//  else if(strcmp(whoami, "zs") == 0)
//  {
//    printf("执行成功,用户为zs\n");
//  }
//  else 
//  {
//    printf("识别错误,强制退出\n");
//    return 1;
//  }
//
//  return 0;
//}


//int main()
//{
//  printf("TEST_ENV=%s\n", getenv("TEST_ENV"));
//  printf("OTHER_ENV=%s\n", getenv("OTHER_ENV"));
//
//  return 0;
//}


int g_val = 10;

int main()
{
  printf("g_val: %d, &g_val: %p\n", g_val, &g_val);
  
  pid_t id = fork();
  if(id == 0)
  {
    while(1)
    {
      printf("子进程:pid = %d, ppid = %d, g_val = %d, &g_val = %p\n", getpid(), getppid(), g_val, &g_val);
      sleep(2);
    }
  }
  else 
  {
    while(1)
    {
      printf("父进程:pid = %d, ppid = %d, g_val = %d, &g_val = %p\n", getpid(), getppid(), g_val, &g_val);
      sleep(2);
    }
  }

  return 0;
}
