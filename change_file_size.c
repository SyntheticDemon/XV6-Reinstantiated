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
  int desired_size = atoi(argv[3]);

  int fd = open(argv[1], O_CREATE | O_RDWR);
  write(fd, argv[2],strlen(argv[2]));
  change_file_size(argv[1], desired_size);
  exit();
}
