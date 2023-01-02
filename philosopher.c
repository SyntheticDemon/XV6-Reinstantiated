#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{

  int fid = atoi(argv[1]);
  while (1)
  {
    if (fid % 2 == 0)
    // solution for dead lock , each philosopher if even first picks up his right chopstick , then his left 
    // vice versa for odd philosophers, in this case no adjacent philosophers cause contention on one chopstick since, they first acquire and wait for a chopstick that could potentially cause dead lock , but that chopstick is already occupied , therefore a dead lock never occurs
    // this way we only grab locks with the even indices as stated in
    // https://stackoverflow.com/questions/55653850/how-to-prevent-deadlock-in-dining-philosopher-c
    {
      printf(1, "%d Even philosopher picking up his right chopstick first \n",fid);
      sem_acquire(fid);
      printf(1, "%d Eeven philosopher picking up his left chopstick afterwards\n",fid);
      sem_acquire((fid + 1) % 5);
    }
    else
    {
      printf(1, "%d Odd philosopher picking up his left chopstick first\n",fid);
      sem_acquire((fid + 1) % 5);
      printf(1, "%d Odd philosopher picking up his right chopstick afterwards\n", fid);
      sem_acquire(fid);
    }
    // If we don't want the stdout printing process not to be preempted before writing it's text fully  we can add a  sleep before the important prints 
    //sleep(1000);  
    printf(1, "chopstick %d and %d was picked up by philosopher with id %d\n", fid, (fid + 1) % 5, fid);
    printf(1, "philosopher %d is eating\n", fid);
    // sleep(1000);
    sem_release(fid);
    sem_release(((fid + 1) % 5));
    printf(1, "chop stick %d and %d was putdown by philosopher with id :%d \n", fid, (fid + 1) % 5, fid);
    printf(1, "philosopher %d is thinking\n", fid);
  }

  exit();
}