#include <stdio.h>

int main(int argc, char* argv[], char* env[])
{
  for(int i = 0; i < argc; i++)
  {
    printf("argv[%d]: %s\n", i, argv[i]);
  }

  printf("\n");

  for(int i = 0; env[i]; i++)
  {
    printf("env[%d]: %s\n", i, env[i]);
  }
  
  printf("这是用户自己编写的程序!\n");

  return 0;
}






