#include <stdio.h>

int main(void)
{
  int max = -101;
  int sum = 0;
  int num = 1;

  while (num != 0)
  {
    scanf("%d", &num);
    sum += num;
    if (num > max && num != 0)
      max = num;
  }
  printf("sum: %d\n", sum);
  printf("maximum: %d\n", max);

  return 0;
}
