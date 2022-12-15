#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#define LOTTERY_TICKETS_BASE_DEFAULT 5
#define CYCLE_THRESHOLD 8000
struct
{
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

void print_float(float xx)
{
  int beg = (int)(xx);
  int fin = (int)(xx * 100) - beg * 100;
  cprintf("%d", beg);
  cprintf(".");
  if (fin < 10)
    cprintf("0");
  cprintf("%d", fin);
}

void print_process_State(enum procstate process_state){
  cprintf("Status :");
  if (process_state == RUNNABLE){
    cprintf("Runnable |");
  }
  else if (process_state ==RUNNING){
    cprintf("Running |");
  }
  else if (process_state == ZOMBIE){
    cprintf("Zombie |");
  }
  else if (process_state== SLEEPING){
    cprintf("Sleeping |");
  }
  else if (process_state ==EMBRYO){
    cprintf("Embryo |");
  }
  else if (process_state == UNUSED){
    cprintf("Unused |");
  }
}

int sys_ps_aux(void)
{
  struct proc *p;
  acquire(&ptable.lock);

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->pid > 0)
    {

      print_process_State(p->state);
      cprintf("Name :%s | PID : %d | Queue %d | Priority %d | Lottary Tickets %d | Age %d |", p->name, p->pid, p->queue, p->priority, p->lottery_tickets, p->age);
      cprintf("Arrival Ratio :  ");
      cprintf("%d", p->arrival_ratio);
      cprintf("|Priority Ratio :  ");
      cprintf("%d", p->priority_ratio);
      cprintf("|Arrival Time :  ");
      cprintf("%d", p->arrival_time);
      cprintf("|Cycles Ratio :  ");
      cprintf("%d", p->cycles_ratio);
      cprintf("|Cycles :  ");
      cprintf("%d", p->cycles);
      cprintf("|Rank :  ");
      print_float(p->rank);
      cprintf("\n");
      cprintf("=============================================================================");
      cprintf("============================================================================\n");
      cprintf("\n");
    }
  }
  release(&ptable.lock);
}

int sys_change_process_queue(void)
{
  int pid;
  int new_queue_id;
  if (argint(0, &pid) < 0)
    return -1;
  if (argint(1, &new_queue_id) < 0)
    return -1;
  int current_time = ticks;
  for (int i = 0; i < NPROC; i++)
  {
    if (ptable.proc[i].pid == pid)
    {
      cprintf("Process arrival time %d\n", current_time - ptable.proc[i].arrival_time);
      acquire(&ptable.lock);
      ptable.proc[i].queue = new_queue_id;
      release(&ptable.lock);
      cprintf("The process is now in queue %d \n", ptable.proc[i].queue);
      break;
    }
  }
}

int sys_change_coef(void)
{
  int pid;
  int arrival_coef;
  int priority_coef;
  int cycle_coef;
  if (argint(0, &pid) < 0)
    return -1;
  if (argint(1, &arrival_coef) < 0)
    return -1;
  if (argint(2, &priority_coef) < 0)
    return -1;
  if (argint(3, &cycle_coef) < 0)
    return -1;
  acquire(&ptable.lock);
  for (int i = 0; i < NPROC; i++)
  {
    if (ptable.proc[i].pid == pid)
    {
      ptable.proc[i].arrival_ratio = arrival_coef;
      ptable.proc[i].cycles_ratio = cycle_coef;
      ptable.proc[i].priority_ratio = priority_coef;
      break;
    }
  }
  release(&ptable.lock);
  return 1;
}

int sys_change_ultra_coef(void)
{
  int arrival_coef;
  int priority_coef;
  int cycle_coef;
  if (argint(0, &arrival_coef) < 0)
    return -1;
  if (argint(1, &priority_coef) < 0)
    return -1;
  if (argint(2, &cycle_coef) < 0)
    return -1;
  acquire(&ptable.lock);

  for (int i = 0; i < NPROC; i++)
  {
    ptable.proc[i].arrival_ratio = arrival_coef;
    ptable.proc[i].cycles_ratio = cycle_coef;
    ptable.proc[i].priority_ratio = priority_coef;
  }
  release(&ptable.lock);
  return 1;
}

int sys_tickets_change(void)
{
  int pid;
  int new_queue_id;
  if (argint(0, &pid) < 0)
    return -1;
  if (argint(1, &new_queue_id) < 0)
    return -1;
  int current_time = ticks;
  for (int i = 0; i < NPROC; i++)
  {
    if (ptable.proc[i].pid == pid)
    {
      cprintf("Process arrival time %d\n", current_time - ptable.proc[i].arrival_time);
      acquire(&ptable.lock);
      ptable.proc[i].queue = new_queue_id;
      release(&ptable.lock);
      cprintf("The process now has  %d tickets \n", ptable.proc[i].lottery_tickets);
      break;
    }
  }
}

static struct proc *initproc;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

void pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Must be called with interrupts disabled
int cpuid()
{
  return mycpu() - cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu *
mycpu(void)
{
  int apicid, i;

  if (readeflags() & FL_IF)
    panic("mycpu called with interrupts enabled\n");

  apicid = lapicid();
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i)
  {
    if (cpus[i].apicid == apicid)
      return &cpus[i];
  }
  panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct proc *
myproc(void)
{
  struct cpu *c;
  struct proc *p;
  pushcli();
  c = mycpu();
  p = c->proc;
  popcli();
  return p;
}

// PAGEBREAK: 32
//  Look in the process table for an UNUSED proc.
//  If found, change state to EMBRYO and initialize
//  state required to run in the kernel.
//  Otherwise return 0.
static struct proc *
allocproc(void)
{
  cprintf("Forking!\n");
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if (p->state == UNUSED)
      goto found;

  release(&ptable.lock);
  return 0;

found:
  // cprintf("Created new process\n");
  p->state = EMBRYO;
  p->pid = nextpid++;

  release(&ptable.lock);

  // Allocate kernel stack.
  if ((p->kstack = kalloc()) == 0)
  {
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe *)sp;

  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint *)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context *)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;
  p->queue = 2;
  p->arrival_time = ticks;
  p->arrival_ratio = 1;
  p->cycles = 0;
  p->cycles_ratio = 1;
  p->age = 0;
  p->priority_ratio = 1;
  p->priority = 1;
  p->rank = 0;
  p->lottery_tickets = LOTTERY_TICKETS_BASE_DEFAULT;
  return p;
}

// PAGEBREAK: 32
//  Set up first user process.
void userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  p = allocproc();

  initproc = p;
  if ((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0; // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  acquire(&ptable.lock);

  p->state = RUNNABLE;

  release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int growproc(int n)
{
  uint sz;
  struct proc *curproc = myproc();

  sz = curproc->sz;
  if (n > 0)
  {
    if ((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  }
  else if (n < 0)
  {
    if ((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  }
  curproc->sz = sz;
  switchuvm(curproc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int fork(void)
{
  int i, pid;
  struct proc *np;
  struct proc *curproc = myproc();

  // Allocate process.
  if ((np = allocproc()) == 0)
  {
    return -1;
  }

  // Copy process state from proc.
  if ((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0)
  {
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = curproc->sz;
  np->parent = curproc;
  *np->tf = *curproc->tf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for (i = 0; i < NOFILE; i++)
    if (curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  pid = np->pid;

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);

  return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void exit(void)
{
  struct proc *curproc = myproc();
  struct proc *p;
  int fd;

  if (curproc == initproc)
    panic("init exiting");

  // Close all open files.
  for (fd = 0; fd < NOFILE; fd++)
  {
    if (curproc->ofile[fd])
    {
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(curproc->parent);

  // Pass abandoned children to init.
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->parent == curproc)
    {
      p->parent = initproc;
      if (p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int wait(void)
{
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();

  acquire(&ptable.lock);
  for (;;)
  {
    // Scan through table looking for exited children.
    havekids = 0;
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    {
      if (p->parent != curproc)
        continue;
      havekids = 1;
      if (p->state == ZOMBIE)
      {
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if (!havekids || curproc->killed)
    {
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock); // DOC: wait-sleep
  }
}

// PAGEBREAK: 42
//  Per-CPU process scheduler.
//  Each CPU calls scheduler() after setting itself up.
//  Scheduler never returns.  It loops, doing:
//   - choose a process to run
//   - swtch to start running that process
//   - eventually that process transfers control
//       via swtch back to the scheduler.
// Random generator code taken from https://github.com/joonlim/xv6/blob/master/random.c
random(void)
{
  // Take from http://stackoverflow.com/questions/1167253/implementation-of-rand
  static unsigned int z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
  unsigned int b;
  b = ((z1 << 6) ^ z1) >> 13;
  z1 = ((z1 & 4294967294U) << 18) ^ b;
  b = ((z2 << 2) ^ z2) >> 27;
  z2 = ((z2 & 4294967288U) << 2) ^ b;
  b = ((z3 << 13) ^ z3) >> 21;
  z3 = ((z3 & 4294967280U) << 7) ^ b;
  b = ((z4 << 3) ^ z4) >> 12;
  z4 = ((z4 & 4294967168U) << 13) ^ b;

  return (z1 ^ z2 ^ z3 ^ z4) * ticks / 2;
}
struct proc *sched_lottery(void)
{
  struct proc *p;
  int all_tickets = 0;
  // sum all the tickets of all processes
  // cprintf("Running lottary \n");
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->state != RUNNABLE || p->queue != 2)
    {
      continue;
    }
    // cprintf("Process count");
    all_tickets += p->lottery_tickets;
  }
  // Do the loterry
  // cprintf("All tickets %d \n", all_tickets);
  if (all_tickets != 0)
  {
    int chosen_ticket = random() % all_tickets;
    int base = 0;
    // cprintf("Chosen ticket %d\n", chosen_ticket);
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    {
      if (p->state != RUNNABLE || p->queue != 2)
      {
        continue;
      }
      int last_base = base;
      base += p->lottery_tickets;
      // cprintf("Range %d to %d\n", last_base, base);
      if (last_base <= chosen_ticket && chosen_ticket <= base)
      {
        // cprintf("process with pid %d won the lottary \n", p->pid);
        return p;
      }
    }
  }
  else
  {
    return 0;
  }
}
struct proc *sched_bjf(void)
{
  struct proc *best_process = 0;
  int best_rank = 1000000;
  struct proc *p;
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->state != RUNNABLE || p->queue != 3)
    {
      continue;
    }
    else
    {
      float rank = (p->priority_ratio * p->priority) + (p->arrival_time * p->arrival_ratio) + (p->cycles * p->cycles_ratio);
      p->rank = rank;
      if (rank <= best_rank)
      {
        best_rank = rank;

        best_process = p;
      }
    }
  }
  return best_process;
}

void scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;

  for (;;)
  {
    // Enable interrupts on this processor.
    sti();

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    int first_level_empty = 1;
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    {
      if (p->cycles >= CYCLE_THRESHOLD)
      {
        if (p->queue)
        {
          p->queue = 1;
        }
      }
    }
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    {
      if (p->state != RUNNABLE)
        continue;
      if (p->queue != 1) // default priority
        continue;
      first_level_empty = 0;
      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      c->proc = p;
      switchuvm(p);
      p->state = RUNNING;
      p->cycles += 0.1;
      if (p->cycles >= CYCLE_THRESHOLD)
      {
        p->queue = 1;
        p->cycles = 0;
      }
      swtch(&(c->scheduler), p->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      c->proc = 0;
    }

    // Switch to the second queue
    int second_queue_empty = 1;
    if (first_level_empty)
    {
      p = sched_lottery();

      // cprintf("Returned from lottary %d", next_to_run->pid);
      // cprintf("%d", p != 0);
      if (p != 0)
      {
        second_queue_empty = 0;
        c->proc = p;
        switchuvm(p);
        p->state = RUNNING;
        p->cycles += 0.1;

        swtch(&(c->scheduler), p->context);
        switchkvm();
        c->proc = 0;
      }
    }
    int third_queue_empty = 1;
    if (first_level_empty && second_queue_empty)
    {
      p = sched_bjf();
      if (p != 0)
      {
        third_queue_empty = 0;
        c->proc = p;
        switchuvm(p);
        p->state = RUNNING;
        p->cycles += 0.1;
        swtch(&(c->scheduler), p->context);
        switchkvm();
        c->proc = 0;
      }
    }
    release(&ptable.lock);
  }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void sched(void)
{
  int intena;
  struct proc *p = myproc();

  if (!holding(&ptable.lock))
    panic("sched ptable.lock");
  if (mycpu()->ncli != 1)
    panic("sched locks");
  if (p->state == RUNNING)
    panic("sched running");
  if (readeflags() & FL_IF)
    panic("sched interruptible");
  intena = mycpu()->intena;
  swtch(&p->context, mycpu()->scheduler);
  mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.
void yield(void)
{
  acquire(&ptable.lock); // DOC: yieldlock
  myproc()->state = RUNNABLE;
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first)
  {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void sleep(void *chan, struct spinlock *lk)
{
  struct proc *p = myproc();

  if (p == 0)
    panic("sleep");

  if (lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if (lk != &ptable.lock)
  {                        // DOC: sleeplock0
    acquire(&ptable.lock); // DOC: sleeplock1
    release(lk);
  }
  // Go to sleep.
  p->chan = chan;
  p->state = SLEEPING;

  sched();

  // Tidy up.
  p->chan = 0;

  // Reacquire original lock.
  if (lk != &ptable.lock)
  { // DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

// PAGEBREAK!
//  Wake up all processes sleeping on chan.
//  The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if (p->state == SLEEPING && p->chan == chan)
      p->state = RUNNABLE;
}

// Wake up all processes sleeping on chan.
void wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int kill(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->pid == pid)
    {
      p->killed = 1;
      // Wake process from sleep if necessary.
      if (p->state == SLEEPING)
        p->state = RUNNABLE;
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

// PAGEBREAK: 36
//  Print a process listing to console.  For debugging.
//  Runs when user types ^P on console.
//  No lock to avoid wedging a stuck machine further.
void procdump(void)
{
  static char *states[] = {
      [UNUSED] "unused",
      [EMBRYO] "embryo",
      [SLEEPING] "sleep ",
      [RUNNABLE] "runble",
      [RUNNING] "run   ",
      [ZOMBIE] "zombie"};
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->state == UNUSED)
      continue;
    if (p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if (p->state == SLEEPING)
    {
      getcallerpcs((uint *)p->context->ebp + 2, pc);
      for (i = 0; i < 10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}
