#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  //chdir("/home/zs");
  //const char *filename = "test.txt";
  //FILE *fp = fopen(filename, "w");   // 以写的方式打开test.txt文件
  //if(fp == NULL)
  //{
  //  perror("fopen");
  //  return 1;
  //}

  //fputs("hello Linux\n", fp);
  //fclose(fp);  // 关闭打开的文件


  //const char *filename = "test.txt";
  //FILE *fp = fopen(filename, "r");   // 以写的方式打开test.txt文件
  //if(fp == NULL)
  //{
  //  perror("fopen");
  //  return 1;
  //}

  //int count = 3;
  //while(count--)
  //{
  //  fputs("hello linux\n", fp);
  //}
  //
  //while(1)
  //{
  //  char buffer[128];
  //  if(!fgets(buffer, sizeof(buffer), fp))
  //    break;
  //  printf("from test.txt: %s\n", buffer);
  //}
  //fclose(fp);  // 关闭打开的文件
  

  //const char *filename = "test.txt";
  //FILE *fp = fopen(filename, "w");   // 以写的方式打开test.txt文件
  //if(fp == NULL)
  //{
  //  perror("fopen");
  //  return 1;
  //}


  const char *filename = "test.txt";
  FILE *fp = fopen(filename, "a");
  if(fp == NULL)
  {
    perror("fopen");
    return 1;
  }

  int count = 4;
  while(count--)
  {
    fputs("hello linux\n", fp);
  }
  fclose(fp);

  return 0;
}

