#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


int main()
{
  //int fd = open("test.txt", O_RDONLY); 
  //if(fd < 0)
  //{
  //  perror("open");
  //}

  //char buffer[128];
  //ssize_t n = read(fd, buffer, sizeof(buffer)-1); // 不计入\0
  //if(n > 0)
  //{
  //  buffer[n] = 0; //手动加上\0
  //  printf("%s\n", buffer);
  //}

  //close(fd);
  

  //close(1);
  //int fd = open("test.txt", O_WRONLY | O_TRUNC);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);

  //close(1);
  //int fd = open("test.txt", O_WRONLY | O_TRUNC);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);

  //close(fd);
  


  //close(1);
  //int fd = open("test.txt", O_WRONLY | O_TRUNC);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);

  //fflush(stdout);
  //close(fd);
  

  // 向显示器打印字符
  printf("hello printf\n");
  fprintf(stdout, "hello fprintf\n");

  const char* s = "hello fputs\n";
  fputs(s, stdout);

  const char* str = "hello write\n";
  write(1, str, strlen(str));

  fork();
  return 0;
}
