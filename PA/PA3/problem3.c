#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
  for (int i = 1; i < argc; )
  {
    if (!strcmp(argv[i], "-o"))
      {printf("-o %s: Place the primary output in file %s.\n", argv[i + 1], argv[i + 1]); i += 2; continue;}
    if (!strcmp(argv[i], "-I"))
      {printf("-I %s: Add the directory %s to the list of directories to be searched for header files during preprocessing.\n", argv[i + 1], argv[i + 1]); i += 2; continue;}


    if (argv[i][1] == 's')
    {
      char *str = argv[i];
      char *std = malloc(10 * sizeof(char));
      char *N = malloc(10 * sizeof(char));
      char *_std = std;
      char *_N = N;

      // 跳过-std=
      str += 5;
      // printf("%s\n", str);
      while (!isdigit(*(std++) = *(str++)));
      str--;
      *(--std) = '\0'; 
      while ((*N++ = *str++) != '\0');
      
      // printf("%s\n", _std);
      // printf("%s\n", _N);
      if (!strcmp(_std, "c")) printf("-std=%s%s: Set the language standard to ISO C%s.\n", _std, _N, _N);
      if (!strcmp(_std, "c++")) printf("-std=%s%s: Set the language standard to ISO C++%s.\n", _std, _N, _N);
      if (!strcmp(_std, "gnu")) printf("-std=%s%s: Set the language standard to GNU dialect of C%s.\n", _std, _N, _N);
      if (!strcmp(_std, "gnu++")) printf("-std=%s%s: Set the language standard to GNU dialect of C++%s.\n", _std, _N, _N);
      free(_std); free(_N);
      i++;
      continue;
    }

    if (strrchr(argv[i], '.') != NULL)
    {
      char *str = argv[i];
      char *_str = str;
      char *postfix = malloc(10 * sizeof(char));
      char *_postfix = postfix;

      str = strrchr(str, '.');
      while ((*postfix++ = *str++) != '\0');

      if (strlen(_postfix) == 2)
      {
        if (_postfix[1] == 'c') printf("%s: C source code as input file.\n", _str);
        if (_postfix[1] == 'h') printf("%s: C/C++ header file as input file.\n", _str);
        if (_postfix[1] == 'C') printf("%s: C++ source code as input file.\n", _str);
      }
      else 
      {
        if (_postfix[1] == 'c') printf("%s: C++ source code as input file.\n", _str);
        if (_postfix[1] == 'h') printf("%s: C++ header file as input file.\n", _str);
      }
      free(_postfix);
      i++;
      continue;
    }

    if (!strcmp(argv[i], "-Wall"))
      printf("-Wall: Enable all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning).\n");
    if (!strcmp(argv[i], "-Wpedantic"))
      printf("-Wpedantic: Issue all the warnings demanded by strict ISO C and ISO C++ and reject all programs that use forbidden extensions.\n");
    if (!strcmp(argv[i], "-Wextra"))
      printf("-Wextra: Enable some extra warning flags that are not enabled by -Wall.\n");
    if (!strcmp(argv[i], "-Werror"))
      printf("-Werror: Make all warnings into errors.\n");

    i++;
  }
  return 0;
}
