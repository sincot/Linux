#include "probar.h"
#include<unistd.h>
#include<string.h>

#define N 101
#define STYLE '='

// 颜色代码定义
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define PURPLE  "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"
#define RESET   "\033[0m"


void FlushProcess(double total, double cur)
{
  // 避免进度条进度大于100%
  if(cur > total)
  {
    cur = total;
  }

  // 进度条的进度
  double rate = cur / total * 100;
  // 进度条buff中=的个数
  int count = (int)rate;  // 取整
  // 提前开好101大小的空间
  char probarbuff[N];
  // 初始化
  memset(probarbuff, '\0', sizeof(probarbuff)/sizeof(char));

  int i = 0;
  for(; i < count; i++)
  {
    probarbuff[i] = STYLE;
  }
  
  // 旋转光标
  static char lable[4] = { '|', '/', '-', '\\' };
  static int index = 0; // FlushProcess函数的调用次数
  // 刷新进度条
  printf(PURPLE"[%-100s][%.1f%%][%c]\r" RESET, probarbuff, rate, lable[index++]);
  index %= 4; //始终控制index在一定的范围内
  fflush(stdout);
  // 下载总量大于等于文件大小总量，即说明下载完毕，需要换行
  if(cur >= total)
  {
    printf("\n");
  }
}


void Probar()
{
  //提前开辟好空间 0~100共101个,所以需要101大小的空间
  char probarbuff[N];
  // 初始化probarbuff数组
  memset(probarbuff, '\0', sizeof(probarbuff)/sizeof(char));
  // 旋转光标的四个状态
  char lable[4] = { '|', '/', '-', '\\' };
  //计算循环的打印次数
  int count = 0;
  // 循环打印进度条
  while(count <= N - 1)
  {
    // 进度条
    printf("[%-100s][%d%%][%c]\r", probarbuff, count, lable[count%4]);
    fflush(stdout);
    // 每循环一次,进度条就增加一个=
    probarbuff[count++] = STYLE;
    // 每打印一次休眠5万微秒
    usleep(50000);
  }
  printf("\n");
}


