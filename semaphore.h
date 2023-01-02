#include "types.h"
#include "spinlock.h"

#define SEMAPHORE_COUNT 5

struct semaphore
{
  int value;
  int locked;
  int philosopher;
  struct spinlock lock;
};
struct semaphore sticks[5];