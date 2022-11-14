#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(int argc, char *argv[])
{
  // upon writing change_file_size
  //<test_file_name> <file_contents> <file_size> changes the file contents to be of length file size in characters
  // const int desired_max_length = atoi(argv[2]);
  // // argv[1] is the file directory
  if (fork() == 0 ){
    if (fork()== 0){
      printf(1, "I am the child with pid %d and the parent is %d\n", getpid(), get_parent_pid());
    }
    else {
      printf(1, "I am the parent with pid %d and my parent is %d\n", getpid(), get_parent_pid());
      // After 3 generations the father will be overtaken by the 1 process thats why we get  the 1 for the chil's parent here 
      // by putting wait cycles in these forks we can cause the 1 process to over take any orphan process which is proof for what i say here
    }
  }
  else{
    printf(1,"I am the grand parent pid :%d\n",getpid());
  }
  exit();
}