#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[])
{
  printf(1,"Changing process tickets for Process with pid %d :%d\n",atoi(argv[1]),atoi(argv[2]));
  tickets_change(atoi(argv[1]),atoi(argv[2]));
  exit();
}