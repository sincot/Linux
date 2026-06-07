#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* env[])
{
  printf("TEST_ENV:%s\n", getenv("TEST_ENV"));
  printf("OTHER_ENV:%s\n", getenv("OTHER_ENV"));

  //char* whoami = getenv("USER");
  //if(whoami == NULL)
  //{
  //  printf("执行失败，不认识该用户\n");
  //}
  //else if(strcmp(whoami, "root") == 0)
  //{
  //  printf("执行失败，非目标用户\n");
  //}
  //else if(strcmp(whoami, "zs") == 0)
  //{
  //  printf("执行成功，用户为zs\n");
  //}
  //else
  //{
  //  printf("识别错误，强制退出\n");
  //  return 1;
  //}

  //extern char** environ;
  //int i = 0;
  //for(; environ[i]; i++)
  //{
  //  printf("environ[%d]->%s\n", i, environ[i]);
  //}


  //int i = 0;
  //for(; env[i]; i++)
  //{
  //  printf("env[%d]->%s\n", i, env[i]);
  //}


  //printf("这是用户自己实现的一个命令!\n");

  //int i = 0;
  //for(i = 0; i <= argc; i++)
  //{
  //  printf("argv[%d]->%s\n", i, argv[i]);
  //}

  //if(argc != 2)
  //{
  //  printf("该命令使用错误!你应该这样使用: %s-a|-b|-c|-d\n", argv[0]);
  //  return 1;
  //}

  //if(strcmp(argv[1], "-a") == 0)
  //{
  //  printf("现在执行的是第一种功能\n");
  //}
  //else if(strcmp(argv[1], "-b") == 0)
  //{
  //  printf("现在执行的是第二种功能\n");
  //}
  //else if(strcmp(argv[1], "-c") == 0)
  //{
  //  printf("现在执行的是第三种功能\n");
  //}
  //else
  //{
  //  printf("现在执行的是该命令的默认功能\n");
  //}


  //printf("argc:%d\n", argc);
  //
  //int i = 0; 
  //for(i = 0; i < argc; i++)
  //{
  //  printf("argv[%d]->%s\n", i, argv[i]);
  //}

  return 0;
}


