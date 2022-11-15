#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#define sys_call_length 24
#define MAX_PROCESS_COUNT 6000
// Fetch the int at addr from the current process.
int call_array[sys_call_length][6000] = {0};
void add_to_call_array(int sys_call_id, int process_id)
{
  for (int i = 0; i < MAX_PROCESS_COUNT; i++)
  {
    if (call_array[sys_call_id][i] == 0)
    {
      // cprintf("process id : %d  sys call id: %d",process_id,sys_call_id);
      call_array[sys_call_id][i] = process_id;
      return;
    }
  }
}

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_getyear(void)
{
  return 1975;
}

void sys_getcallers(void)
{
  int sys_id;
  if (argint(0, &sys_id) < 0)
    return;
  else
  {
    cprintf("Process's that have called the system call with id %d:", sys_id);
    for (int i = 0; i < MAX_PROCESS_COUNT; i++)
    {
      if (call_array[sys_id][i] != 0)
      {
        cprintf("%d,", call_array[sys_id][i]);
      }
      else
      {
        cprintf("\n");
        return;
      }
    }
  }
}
int sys_get_parent_pid(void)
{
  return myproc()->parent->pid;
}

int sys_find_largest_prime_factor(void)
{
  int n = myproc()->tf->esp;
  int dividant = 2;
  int answer = 0, max_factor;
  while (n != 0)
  {
    if (n % dividant != 0)
    {
      dividant = dividant + 1;
    }
    else
    {
      max_factor = n;
      n = n / dividant;
      if (n == 1)
      {
        answer = 1;
        break;
      }
    }
  }
  return max_factor;
}