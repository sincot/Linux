#include "mystdio.h"
#include <unistd.h>

int main()
{
  myFILE *fp = myfopen("test.txt", "w");
  if(fp == NULL)
  {
    perror("myfopen");
    return 1;
  }


  const char* buffer = "hello linux\n";
  int count = 20;
  while(count--)
  {
    myfputs(buffer, fp);
    sleep(2);
    printf("debug: outbuffer = %s, pos = %d, count: %d\n", fp->outbuffer, fp->pos, count);
  }

  myfclose(fp);

  return 0;
}


