#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool
is_palindrome(const char *str, uint32_t len)
{
  for (uint32_t i = 0; i < len / 2; ++i)
    if (str[i] != str[len - 1 - i]) return false;
  return true;
}

int 
main(void)
{
  uint32_t N = 0;
  scanf("%u", &N);

  for (uint32_t i = 0; i < N; ++i)
  {
    uint32_t len;
    scanf("%u", &len);
    char *str = malloc(len * sizeof(char) + 1);
    scanf("%s", str);
    bool flag = is_palindrome(str, len);
    printf(flag ? "Yes\n" : "No\n");
    free(str);
  }

  return 0;
}
