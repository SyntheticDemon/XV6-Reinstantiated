#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int check_prime(int number)
{
  int z;
  if (number == 1 ){
    return 0;
  }
  for (z = 2; z * z <= number; z++)
  {
    if (number % z == 0)
    {
      return 0;
    }
  }
  return 1;
}
char *itoa(int value, char *result, int base)
{
  // check that the base if valid
  if (base < 2 || base > 36)
  {
    *result = '\0';
    return result;
  }

  char *ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do
  {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
  } while (value);
  if (tmp_value < 0)
    *ptr++ = '-';
  *ptr-- = '\0';
  while (ptr1 < ptr)
  {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

int main(int argc, char *argv[])
{
  int i,bigger,fd,smaller;
  if (atoi(argv[1]) > atoi(argv[2])){
    bigger=atoi(argv[1]);
    smaller=atoi(argv[2]);
  }
  else {
    bigger=atoi(argv[2]);
    smaller=atoi(argv[1]);
  }
  unlink("primes.txt");
  fd = open("primes.txt", O_CREATE | O_RDWR);
  if (fd >= 0)
  {
    printf(1, "Creating File Succeeded\n");
  }
  else
  {
    printf(1, "Creating File Failed Exiting\n");
    exit();
  }
  for (i = smaller; (smaller <= i) & (i <= bigger); i++)
  {
    char temp_output[512];
    itoa(i,temp_output,10);
    if (check_prime(i)){
      write(fd,(void*)temp_output, sizeof(i));
    }
  }
  exit();
}
