#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[])
{
  change_coef(atoi(argv[1]),atoi(argv[2]),argv[3],argv[4]);
  exit();
}