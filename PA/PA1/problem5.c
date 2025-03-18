#include <stdio.h>
#include <math.h>

int main(void)
{
  int a, b, c;
  scanf("%d %d %d", &a, &b, &c);

  if (a != 0)
  {
    double discriminant =  pow(b, 2) - 4 * a * c;
    if (discriminant < 0)
    {
      printf("No solution.\n");
      goto done;
    }

    double x1 = ((-b + sqrt(discriminant)) / (2 * a));
    double x2 = ((-b - sqrt(discriminant)) / (2 * a));
    if (!discriminant)
      printf("x1 = x2 = %.3lf\n", x1);
    else 
      printf("x1 = %.3lf, x2 = %.3lf\n", x2, x1);
  }
  else if (a == 0 && b != 0)
    printf("x = %.3lf\n", -((double)c/b));
  else if (a == 0 && b == 0 && c != 0)
    printf("No solution.\n");
  else if (a == 0 && b == 0 && c == 0)
    printf("x\\in\\mathbb{R}\n");
  
done:
  return 0;
}
