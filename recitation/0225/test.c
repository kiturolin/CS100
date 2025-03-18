#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

int ccvi(char);

int
ccvi(char x)
{
  return x - '0';
}

int
main(void)
{
  printf("%d\n", (3 / -2));

  return 0;
}
