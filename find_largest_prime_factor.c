#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[])
{
  printf(1,"Largest prime factor for %d :%d\n",atoi(argv[1]),find_largest_prime_factor(atoi(argv[1])));
  exit();
}