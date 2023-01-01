#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void reverse_str(char str[], int len)
{
  int start, end;
  char temp;
  for (start = 0, end = len - 1; start < end; start++, end--)
  {
    temp = *(str + start);
    *(str + start) = *(str + end);
    *(str + end) = temp;
  }
}

char *itoa(int num, char *str, int base)
{
  int i = 0;
  int isNegative = 0;
  if (num == 0)
  {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }
  if (num < 0 && base == 10)
  {
    isNegative = 1;
    num = -num;
  }
  while (num != 0)
  {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }
  if (isNegative)
    str[i++] = '-';
  str[i] = '\0';
  reverse_str(str, i);

  return str;
}
int main(int argc, char *argv[])
{
  for (int i = 0; i < 5; i++)
  {
    sem_init(i, 1);
  }
  for (int i = 0; i < 5; i++)
  {
    char * h;
    exec("philosopher.c", itoa(i,h,10));
  }

  exit();
}
