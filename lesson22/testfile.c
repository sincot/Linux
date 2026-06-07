#include <stdio.h>
#include <unistd.h>

// 定义一批宏,这些宏的特点是只有一个比特位是1
//#define ONE (1<<0)    // 0000 0001
//#define TWO (1<<1)    // 0000 0010
//#define THREE (1<<2)  // 0000 0100
//#define FOUR (1<<3)   // 0000 1000
//
//// & — 有0则0，同1则1
//// | — 有1则1，同0则0
//void Print(int flag) // 0011 1011 0110
//{
//  if(flag & ONE) { printf("one\n"); }     // 0011 & 0001 = 0001  // 1011 & 0001 = 0001  // 0110 & 0001 = 0000
//  if(flag & TWO) { printf("two\n"); }     // 0011 & 0010 = 0010  // 1011 & 0010 = 0010  // 0110 & 0010 = 0010
//  if(flag & THREE) { printf("three\n"); } // 0011 & 0100 = 0000  // 1011 & 0100 = 0000  // 0110 & 0100 = 0100
//  if(flag & FOUR) { printf("four\n"); }   // 0011 & 1000 = 0000  // 1011 & 1000 = 1000  // 0110 & 1000 = 0000
//}
//
//int main()
//{
//  Print(ONE);           // 打印 one 0001
//  printf("\n");
//  Print(ONE|TWO);       // 打印one two 0001|0010=0011
//  printf("\n");
//  Print(ONE|TWO|FOUR);  // 打印 one two four 0001|0010|1000=1011
//  printf("\n");
//  Print(TWO|THREE);     // 打印 two three 0010|0100=0110
//
//
//  return 0;
//}


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
  //// 打开文件，以写的方式打开
  //int fd = open("test.txt", O_WRONLY);

  //if(fd < 0)
  //{
  //  // 创建失败，返回-1
  //  perror("open");
  //  return 0;
  //}
  

  //// 打开文件，以写和创建的方式打开
  //int fd = open("test.txt", O_WRONLY | O_CREAT);

  //if(fd < 0)
  //{
  //  perror("open");
  //  return 0;
  //}

  // 打开文件，以写和创建的方式打开，权限rw-rw-rw-
  //int fd = open("test.txt", O_WRONLY | O_CREAT, 0666);

  //if(fd < 0)
  //{
  //  perror("open");
  //  return 0;
  //}

  //// 将umask设置为0
  //umask(0);
  //// 打开文件，以写和创建的方式打开，权限rw-rw-rw-
  //int fd = open("test.txt", O_WRONLY | O_CREAT, 0666);

  //if(fd < 0)
  //{
  //  perror("open");
  //  return 0;
  //}

  //// 将umask设置为0
  //umask(0);
  //// 打开文件，以写和创建的方式打开，权限rw-rw-rw-
  //int fd = open("test.txt", O_WRONLY | O_CREAT, 0666);

  //if(fd < 0)
  //{
  //  perror("open");
  //  return 0;
  //}

  //printf("fd: %d\n", fd);


  //// 将umask设置为0
  //umask(0);
  //// 打开文件，以写和创建的方式打开，权限rw-rw-rw-
  //int fd = open("test.txt", O_WRONLY | O_CREAT, 0666);

  //if(fd < 0)
  //{
  //  perror("open");
  //}
  //printf("fd: %d\n", fd);

  //const char* buffer = "hello linux";
  //write(fd, buffer, strlen(buffer));

  //close(fd);

  //// 将umask设置为0
  //umask(0);
  //// 打开文件，以写和创建的方式打开，权限rw-rw-rw-
  //int fd = open("test.txt", O_WRONLY | O_CREAT, 0666);

  //if(fd < 0)
  //{
  //  perror("open");
  //}
  //printf("fd: %d\n", fd);

  //const char* buffer = "664";
  //write(fd, buffer, strlen(buffer));

  //close(fd);


  //// 将umask设置为0
  //umask(0);
  //// 打开文件，以写和创建的方式打开，权限rw-rw-rw-
  //int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

  //if(fd < 0)
  //{
  //  perror("open");
  //}
  //printf("fd: %d\n", fd);

  //const char* buffer = "664";
  //write(fd, buffer, strlen(buffer));

  //close(fd);


  //// 将umask设置为0
  //umask(0);
  //// 打开文件，以写和创建的方式打开，权限rw-rw-rw-
  //int fd = open("test.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);

  //if(fd < 0)
  //{
  //  perror("open");
  //}
  //printf("fd: %d\n", fd);

  //const char* buffer = "775";
  //write(fd, buffer, strlen(buffer));

  //close(fd);

  //umask(0);

  //int fd1 = open("test1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
  //int fd2 = open("test2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
  //int fd3 = open("test3.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);

  //printf("fd1: %d\n", fd1);
  //printf("fd2: %d\n", fd2);
  //printf("fd3: %d\n", fd3);

  //// 向显示器打印数据
  //const char* buffer = "hello linux";
  //printf("hello linux\n");
  //// 向标准输出写入字符串
  //write(1, buffer, strlen(buffer));
  //fputs(buffer, stdout);
  

  //printf("stdin->fd: %d\n", stdin->_fileno);
  //printf("stdout->fd: %d\n", stdout->_fileno);
  //printf("stderr->fd: %d\n", stderr->_fileno);


  //int fd = open("test1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  //printf("fd: %d\n", fd);
 


//*************重定向**********************************//
  // 关闭0号文件
  //close(0);
  //int fd = open("test1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  //printf("fd: %d\n", fd);


  //close(0);
  //close(2);
  //close(1);
  //int fd1 = open("test1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  //printf("fd1: %d\n", fd1);

  //int fd2 = open("test2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  //printf("fd2: %d\n", fd2);

  //int fd3 = open("test3.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  //printf("fd3: %d\n", fd3);


  //close(1);
  //int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //printf("fd: %d\n", fd);
  //close(fd);

  //close(1);
  //int fd = open("test.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
  //printf("fd: %d\n", fd);

  //close(0);
  //char buffer[128];
  //fgets(buffer, sizeof(buffer), stdin);
  //printf("%s\n", buffer);


  //close(0);
  //int fd = open("test.txt", O_RDONLY);
  //printf("fd: %d\n", fd);

  //char buffer[128];
  //fgets(buffer, sizeof(buffer), stdin);
  //printf("%s\n", buffer);


  // 输入重定向
  //int fd = open("test.txt", O_RDONLY);
  //printf("fd: %d\n", fd);
  //dup2(fd, 0);

  //char buffer[128];
  //fgets(buffer, sizeof(buffer), stdin);
  //printf("%s\n", buffer);

  // 输出重定向
  //int fd = open("test.txt", O_WRONLY | O_TRUNC);
  //printf("fd: %d\n", fd);
  //dup2(fd, 1);
  //
  //printf("664775664775\n");
  //printf("664775664775\n");
  //printf("664775664775\n");
  //printf("664775664775\n");
  //printf("664775664775\n");
  //fprintf(stdout, "664775664775\n");

  //int fd = open("test.txt", O_WRONLY | O_APPEND);
  //printf("fd: %d\n", fd);
  //dup2(fd, 1);

  //printf("777777777\n");


  return 0;
}



