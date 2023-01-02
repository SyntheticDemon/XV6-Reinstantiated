#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{

  int fid = atoi(argv[1]);
  while (1)
  {
    if (fid % 2 == 0)
    // solution for dead lock , we remove the cycle by only executing those whose id is
    // this way we only grab locks with the even indices as stated in
    // https://stackoverflow.com/questions/55653850/how-to-prevent-deadlock-in-dining-philosopher-c
    {
      sem_acquire(fid);
      sem_acquire((fid + 1) % 5);
    }
    else
    {
      sem_acquire((fid + 1) % 5);
      sem_acquire(fid);
    }
    printf(1, "chopstick %d and %d was picked up by philosopher with id %d\n", fid, (fid + 1) % 5, fid);
    printf(1, "philosopher %d is eating\n", fid);
    sem_release(fid);
    sem_release(((fid + 1) % 5));
    printf(1, "chop stick %d and %d was putdown by philosopher with id :  %d ", fid, (fid + 1) % 5, fid);
    printf(1, "philosopher %d is thinking\n", fid);
  }

  exit();
}