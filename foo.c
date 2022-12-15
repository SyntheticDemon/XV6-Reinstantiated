#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[])
{
  for (int j = 1; j < 5; j++)
  {
    if (fork() > 0)
    {
      int offset = 0;

      for (int i = 1; i < 100; i++)
      {
        for (int t = 1; t < 100; t++)
        {
          for (int z = 1; z < 100; z++)
          {
            for (int f = 1; f < 100; f++)
            {
              for (int n = 1; n < 100; n++)
              {
                sleep(5);
                offset += z * i * t * f * n;
                // int fd= open("here.txt",O_WRONLY);
                // write(fd,"%d \n",offset);
              }
            }
          }
        }
      }
    }
  }
  for (int i = 0; i < 5; i++)
  {
    wait();
  }
  
  exit();
}