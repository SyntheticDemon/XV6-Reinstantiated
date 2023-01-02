#include "types.h"
#include "stat.h"
#include "user.h"

int single_digit_to_char(int i)
{
  return i + 48;
}

int main(int argc, char *argv[])
{

  for (int i = 0; i < 6; i++)
  {
    // initialize semaphores
    sem_init(i, 1);
    // since 1 philosopher can get a chopstick at a time we have to intiialize our semaphores with the 1  value (as if they are a binray semaphore indiciating if a chopstick is acquired or not )
  }

  for (int i = 0; i < 5; i++)
  {
    int fork_r = fork();
    if (fork_r == 0)
    {
      char arg[1];
      arg[0] = single_digit_to_char(i);
      char *passing_argv[] = {"philosopher", arg, 0};
      exec("philosopher", passing_argv);
    }
  }
  for (int i = 0; i < 5; i++)
    wait();
  exit();
}