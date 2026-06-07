#include "probar.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

double gtotal = 1024.0; // 下载的文件的大小
double speed = 1.0;    // 网速1.0

// 函数指针
typedef void (*call_back) (double total, double cur);

// 网络浮动
double SpeedFloat(double start, double range) // range为浮动范围
{
  // 随机数为整数，去浮动范围的整数部分
  int int_range = range;
  // 生成浮动范围内的随机数 模int_range
  // range - int_range 是浮动范围的小数部分
  return start + rand()%int_range + (range - int_range);
}

// 下载场景
void DownLoad(double total, call_back cb)
{
  // 随机数种子
  srand(time(NULL));
  // 当前下载的总量默认为0.0
  double cur = 0.0;
  // 下载---下载总量等于文件的大小时，也需要刷新进度条
  while(cur <= total)
  {
    // 进度条的刷新
    cb(total, cur);
    cur += SpeedFloat(speed, 6.6); // 模拟下载
    if(cur > total)
    {
      cur = total; // 模拟下载完成
      cb(total, cur);
      break;
    }
    usleep(50000); // 网络延迟
  }
}

int main()
{
  //Probar();
  DownLoad(490, FlushProcess);
  return 0;
}


