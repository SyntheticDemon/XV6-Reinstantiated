#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[])
{
  int philosopher_id = atoi(argv[1]);
  while (1)
  {
    printf(1, "%s : %d \n", "Thinking", philosopher_id);
    sem_acquire(philosopher_id % 5);
    printf(1, "%s : %d \n", "Acquiring First", philosopher_id);
    sem_acquire((philosopher_id + 1) % 5);
    printf(1, "%s : %d \n", "Eating", philosopher_id);
    sem_release(philosopher_id % 5);
    printf(1, "%s : %d", "Released first", philosopher_id);
    sem_release((philosopher_id + 1) % 5);
    printf(1, "%s : %d", "Released seocnd", philosopher_id);
  }
  exit();
}
