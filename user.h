struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int *);
int write(int, const void *, int);
int read(int, void *, int);
int close(int);
int kill(int);
int exec(char *, char **);
int open(const char *, int);
int mknod(const char *, short, short);
int unlink(const char *);
int fstat(int fd, struct stat *);
int link(const char *, const char *);
int mkdir(const char *);
int chdir(const char *);
int dup(int);
int getpid(void);
char *sbrk(int);
int sleep(int);
int uptime(void);
int sem_release(int i);
int sem_acquire(int i);
int sem_init(int i, int v);

// ulib.c
int stat(const char *, struct stat *);
char *strcpy(char *, const char *);
void *memmove(void *, const void *, int);
char *strchr(const char *, char c);
int strcmp(const char *, const char *);
void printf(int, const char *, ...);
char *gets(char *, int max);
uint strlen(const char *);
void *memset(void *, int, uint);
void *malloc(uint);
void free(void *);
int atoi(const char *);
int getyear(void);
int get_parent_pid(void);
int ps_aux(void);
void getcallers(int sys_id);
int change_file_size(const char *file_path, int desired_size);
int change_process_queue(int pid, int queue_id);
int tickets_change(int pid, int tickets);
int change_ultra_coef(int priority_coef, int arrival_ratio, int cycles_ratio);
int change_coef(int pid, int priority_coef, int arrival_ratio, int cycles_ratio, int priority);
// semaphore
