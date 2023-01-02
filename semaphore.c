#include "semaphore.h"
#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"

int sys_sem_init(void)
{
    int i;
    int v;
    if (argint(0, &i) < 0)
        return -1;
    if (argint(1, &v) < 0)
        return -1;
    acquire(&sticks[i].lock);
    // so that we are not preempted during initialization
    if (sticks[i].locked == 0)
    {
        sticks[i].locked = 1;
        sticks[i].value = v;
        sticks[i].philosopher = -1;
    }
    else
    {
        release(&sticks[i].lock);
        return -1;
    }
    release(&sticks[i].lock);
    return 0;
}

int sys_sem_acquire(void)
{
    int i;
    if (argint(0, &i) < 0)
        return -1;
    // so that we are preempted during acquisition
    acquire(&sticks[i].lock);
    if (sticks[i].value >= 1)
    {
        sticks[i].value = sticks[i].value - 1;
        sticks[i].philosopher = i;
    }
    else
    {
        while (sticks[i].value < 1)
        {
            sleep(&sticks[i], &sticks[i].lock);
        }
        sticks[i].value = sticks[i].value - 1;
        sticks[i].philosopher = i;
    }
    // so that we are preempted during acquisition
    release(&sticks[i].lock);

    return 0;
}

int sys_sem_release(void)
{
    int i;
    if (argint(0, &i) < 0)
        return -1;
    acquire(&sticks[i].lock);
    sticks[i].value = sticks[i].value + 1;
    sticks[i].philosopher = -1;
    wakeup(&sticks[i]);
    release(&sticks[i].lock);
    return 0;
}