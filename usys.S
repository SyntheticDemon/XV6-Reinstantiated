#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
SYSCALL(exit)
SYSCALL(wait)
SYSCALL(pipe)
SYSCALL(sem_init)
SYSCALL(sem_acquire)
SYSCALL(sem_release)
SYSCALL(read)
SYSCALL(write)
SYSCALL(find_largest_prime_factor)
SYSCALL(change_process_queue)
SYSCALL(tickets_change)
SYSCALL(close)
SYSCALL(ps_aux)
SYSCALL(change_coef)
SYSCALL(change_ultra_coef)
SYSCALL(kill)
SYSCALL(exec)
SYSCALL(open)
SYSCALL(getyear)
SYSCALL(getcallers)
SYSCALL(get_parent_pid)
SYSCALL(change_file_size)
SYSCALL(mknod)
SYSCALL(unlink)
SYSCALL(fstat)
SYSCALL(link)
SYSCALL(mkdir)
SYSCALL(chdir)
SYSCALL(dup)
SYSCALL(getpid)
SYSCALL(sbrk)
SYSCALL(sleep)
SYSCALL(uptime)
