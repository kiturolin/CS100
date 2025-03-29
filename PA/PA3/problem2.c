#include <stddef.h>
#include <stdio.h>
#include <string.h>

int hw3_islower(int ch);
int hw3_isupper(int ch);
int hw3_isalpha(int ch);
int hw3_isdigit(int ch);
int hw3_tolower(int ch);
int hw3_toupper(int ch);

size_t hw3_strlen(const char *str);
char *hw3_strchr(const char *str, int ch);
char *hw3_strcpy(char *dest, const char *src);
char *hw3_strcat(char *dest, const char *src);
int hw3_strcmp(const char *lhs, const char *rhs);

int 
hw3_islower(int ch)
{
  return ch >= 97 && ch <= 122;
}

int 
hw3_isupper(int ch)
{
  return ch >= 65 && ch <= 90;
}

int 
hw3_isalpha(int ch)
{
  return hw3_islower(ch) || hw3_isupper(ch);
}

int 
hw3_isdigit(int ch)
{
  return ch >= 48 && ch <= 57;
}

int 
hw3_tolower(int ch)
{
  return hw3_isupper(ch) ? ch + 32 : ch;
}

int 
hw3_toupper(int ch)
{
  return hw3_islower(ch) ? ch - 32 : ch;
}

size_t 
hw3_strlen(const char *str)
{
  size_t len = 0;
  for(; *str++ != '\0'; len++);
  return len;
}

char *
hw3_strchr(const char *str, int ch)
{
  for (int i = 0; ;++i)
  {
    if (*(str + i) == ch) {return (char *)str + i;}
    if (*(str + i) == '\0') {return NULL;}
  }
}

char *
hw3_strcpy(char *dest, const char *src)
{
  char *dest_ = dest;
  for (;(*dest++ = *src++) != '\0' ;);
  return dest_;
}

int
hw3_strcmp(const char *lhs, const char *rhs)
{
  for (;*lhs != '\0' && *rhs != '\0'; ++lhs, ++rhs)
  {
    if (*lhs != *rhs)
      return *lhs < *rhs ? -1 : 1;
  }
  // 此时lhs rhs中有一个数为'\0'
  if (*lhs == *rhs)
    return 0;
  else
    return *lhs < *rhs ? -1 : 1;
}

char *
hw3_strcat(char *dest, const char *src)
{
  hw3_strcpy(dest + hw3_strlen(dest), src); 
  return dest;
}


int 
main(void)
{
  char a[20] = "abced";
  char b[10] = "fghijk";

  printf("%s\n", hw3_strcat(a, b));
  printf(hw3_strchr(a, '\0') == NULL ? "True\n" : "False\n");
  printf("%d\n", hw3_strcmp("abc", "abcd"));
  return 0;
}
   
