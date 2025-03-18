#include <stdio.h>

int 
min_element(int *array, int l, int r) 
{
  int pos = l;
  while (l < r) 
  {
    if (array[l] < array[pos])
      pos = l;
    ++l;
  }
  return pos;
}

void 
swap(int *pa, int *pb) 
{
  int tmp = *pa;
  *pa = *pb;
  *pb = tmp;
}

void 
sort(int *array, int n) 
{
  for (int i = 0; i < n - 1; ++i) 
  {
    int min_pos = min_element(array, i, n);
    swap(&array[i], &array[min_pos]);
  }
}

int 
inverse(int num, int *arr)
{
  int a0 = num % 10;
  int a1 = (num / 10) % 10;
  int a2 = (num / 100) % 10;
  int a3 = num / 1000;   
  
  if (arr != NULL)
  {
    arr[0] = a0;
    arr[1] = a1;
    arr[2] = a2;
    arr[3] = a3;
  }

  int new_num = a0 * 1000 + a1 * 100 + a2 * 10 + a3;
  return new_num;
}

int
rearrange(int num)
{
  int arr[4] = {0};
  inverse(num, arr);
  sort(arr, 4);
  int descend_num = arr[3] * 1000 + arr[2] * 100 + arr[1] * 10 + arr[0];
  return descend_num;
}


void 
do_6174(int num)
{
  num = rearrange(num);
  int new_num = inverse(num, NULL);
  if (new_num == num)
  {
    printf("%d is a repdigit.\n", num);
    return ;
  }
  // n1 > n2
  int n1 = new_num > num ? new_num : num;
  int n2 = new_num > num ? num : new_num;
  int n3 = n1 - n2;
  printf("%d - %d = %d\n", n1, n2, n3);
  if (n3 == 6174)
    return ;
  do_6174(n3);
}


int 
main(void)
{
  int N;
  int num;
  scanf("%d", &N);
  for (int i = 0; i < N; ++i)
  {
    scanf("%d", &num);
    if (num == 6174)
      continue; 
    if (num > 9999)
    {
      printf("%d contains more than 4 digits.\n",num);
      continue;
    }
    do_6174(num);
  }

  return 0;
}

