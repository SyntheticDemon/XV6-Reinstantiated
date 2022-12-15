#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[])
{
  printf(1,"Changing process queue for Process with pid %d :%d\n",atoi(argv[1]),atoi(argv[2]));
  change_process_queue(atoi(argv[1]),atoi(argv[2]));
  exit();
}