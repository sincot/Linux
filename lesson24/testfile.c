#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main()
{
  // 向标准输出写入
  printf("这是一条正常的信息\n");
  fprintf(stdout, "这也是一条正常的信息\n");
  const char* s1 = "这是一条正常消息,write\n";
  write(1, s1, strlen(s1));

  // 向标准错误写入
  fprintf(stderr, "这是一条错误细信息\n");
  const char* s2 = "这是一条错误信息,write\n";
  write(2, s2, strlen(s2));
  perror("perror");

  return 0;
}
