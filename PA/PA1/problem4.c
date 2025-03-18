#include <stdio.h>
#include <stdbool.h>

int main(void)
{
  char op;
  char yn;
  double x, y;
  
  bool next_calc = false;

  do 
  {
    scanf("%lf", &x); 
    scanf(" %c", &op); 
    scanf("%lf", &y); 
    
    switch(op)
    {
      case '+':
        printf("%lf + %lf == %lf\n", x, y, x + y);
        break;
      case '-':
        printf("%lf - %lf == %lf\n", x, y, x - y);
        break;
      case '*':
        printf("%lf * %lf == %lf\n", x, y, x * y);
        break;
      case '/':
        printf("%lf / %lf == %lf\n", x, y, x / y);
        break;
      default:
        printf("Unknown operator!\n");
        break;
    }
    printf("Do you want to continue? (y/n)\n");
    scanf(" %c", &yn); 
    next_calc = (yn == 'y');
  }while (next_calc);

  return 0;
}
