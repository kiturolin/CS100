#include <stdio.h>

#define MAX_DEGREE 30

int main(void)
{
  int degree = 0;
  int m = 0;
  double x = 0;
  double val = 0;
  double coeffs[MAX_DEGREE + 1] = {0};

  scanf("%d", &degree);

  for (int i = 0; i <= degree; i++)
    scanf("%lf", coeffs + i);
  
  scanf("%d", &m);

  while(m-- > 0)
  {
    val = 0;
    double p = 1;
    scanf("%lf", &x);
    for (int j = 0; j <= degree; j++)
    {
      val += coeffs[j] * p;
      p *= x;
    }
    printf("%.3lf\n", val);
  }
  return 0;
}
