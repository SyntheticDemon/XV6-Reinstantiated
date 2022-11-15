#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
int main(int argc, char *argv[])
{
    getcallers(SYS_fork);
    getcallers(SYS_wait);
    getcallers(SYS_write);
    exit();
}
