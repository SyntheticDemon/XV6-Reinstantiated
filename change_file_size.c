#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#define MAX_FILE_CHAR_SIZE 8000
#define NUL '\0'
int main(int argc, char *argv[])
{
  // const int desired_max_length = atoi(argv[2]);
  // // argv[1] is the file directory
  int desired_size=atoi(argv[2]);
  // int fd = open(argv[1], O_RDWR);
  // char fb[MAX_FILE_CHAR_SIZE];
  // if (fd >= 0)
  // {
  //   printf(1,"Found file , changing the contents\n");
  // }
  // else
  // {
  //   printf(1, "Could not find file exiting,\n");
  //   exit();
  // }
  // read(fd, fb, MAX_FILE_CHAR_SIZE * sizeof(char));
  // printf(1,"%s",fb);
  // // int file_length = strlen(fb);
  // // if (strlen(fb) == desired_max_length ){
  // //   printf(1,"File is already of that length Exiting!");
  // //   exit();
  // // }
  // // if (file_length > desired_max_length){
  // //   write(fd,fb,desired_max_length * sizeof(char));
  // // }
  // // else{
  // //   char *fb2;
  // //   fb2 = malloc(desired_max_length * sizeof(char));

  // //   for (int i = 0; i < desired_max_length; i++)
  // //   {
  // //     if (i <= strlen(fb)){
  // //       fb2[i]=fb[i];
  // //     }
  // //     else{
  // //       fb2[i]='\0';
  // //     }
        
  // //   }
  // //   write(fd, fb2, strlen(fb2) * sizeof(char));

  // // }  
  printf(1,"%d",change_file_size(argv[1],desired_size));
  exit();
}
