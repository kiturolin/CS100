#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#define BOARD_COLS 15
#define BOARD_ROWS 15

char board[BOARD_ROWS][BOARD_COLS];
uint32_t win_x = 16;
uint32_t win_y = 16;
uint32_t x_cnt = 0;
uint32_t o_cnt = 0;
bool x_next;  // 下一步是否是x
bool already_win_flag = false;

enum directions{
  HORIZON,
  VERTICAL,
  LDIAG,
  RDIAG
};

bool 
in_board(uint8_t x, uint8_t y)
{
  return (x < BOARD_ROWS) && (y < BOARD_COLS);
}

void
check(char c, char *arr, bool is_forward, uint8_t direction, uint8_t x, uint8_t y)
{
  uint8_t cnt = 0;
  int8_t dot_pos = -1;
  for (int i = 0; i < 4; i++)
  {
    char c1 = arr[i];
    if (c1 == c)
      ++cnt;
    else if(c1 == '.')
     dot_pos = i;
  }
  if (cnt == 4)
  {
    printf("AlreadyWin!\n");
    already_win_flag = true;
    return ;
  }
  else if (cnt == 3 && dot_pos != -1 && ((x_next && c == 'X') || (!x_next && c == 'O')))
  {
    switch (direction) {
      case HORIZON: 
        if (is_forward)
          {win_x = x; win_y = y + (dot_pos + 1);}
        else 
          {win_x = x; win_y = y - (dot_pos + 1);}
        break;
      case VERTICAL: 
        if (is_forward)
          {win_x = x + (dot_pos + 1); win_y = y;}
        else 
          {win_x = x - (dot_pos + 1); win_y = y;}
        break; 
      case RDIAG: 
        if (is_forward)
          {win_x = x + (dot_pos + 1); win_y = y + (dot_pos + 1);}
        else 
          {win_x = x - (dot_pos + 1); win_y = y - (dot_pos + 1);}
        break;
      case LDIAG: 
        if (is_forward)
          {win_x = x + (dot_pos + 1); win_y = y - (dot_pos + 1);}
        else 
          {win_x = x - (dot_pos + 1); win_y = y + (dot_pos + 1);}
        break;
    } 
  }
}

void
line_search(uint8_t x, uint8_t y, uint8_t direction)
{
  if (already_win_flag)
    return ;
  char c = board[x][y];
  char forward[4] = {0};
  char backward[4] = {0};
  switch (direction) {
    case HORIZON:
      for(int i = 1; i < 5 && in_board(x, y + i); ++i)
        forward[i - 1] = board[x][y + i];
      for(int i = 1; i < 5 && in_board(x, y - i); ++i)
        backward[i - 1] = board[x][y - i];
      break;
    case VERTICAL:
      for(int i = 1; i < 5 && in_board(x + i, y); ++i)
        forward[i - 1] = board[x + i][y];
      for(int i = 1; i < 5 && in_board(x - i, y); ++i)
        backward[i - 1] = board[x - i][y];
      break;
    case RDIAG:
      for(int i = 1; i < 5 && in_board(x + i, y + i); ++i)
        forward[i - 1] = board[x + i][y + i];
      for(int i = 1; i < 5 && in_board(x - i, y - i); ++i)
        backward[i - 1] = board[x - i][y - i];
      break;
    case LDIAG:
      for(int i = 1; i < 5 && in_board(x + i, y - i); ++i)
        forward[i - 1] = board[x + i][y - i];
      for(int i = 1; i < 5 && in_board(x - i, y + i); ++i)
        backward[i - 1] = board[x - i][y + i];
      break;
  }
  check(c, forward, true, direction, x, y);
  check(c, backward, false, direction, x, y);
}

void
search(uint8_t x, uint8_t y)
{
  enum directions direction; 
  for (direction = HORIZON; direction <= RDIAG; ++direction)
  {
    line_search(x, y, direction);
    if (already_win_flag)
      break;
  }
}

void
traverse()
{
  char c;
  for (int i = 0; i < BOARD_ROWS; ++i)
  {
    for (int j = 0; j < BOARD_COLS; ++j)
    {
      c = board[i][j]; 
      if (c == 'X' || c == 'O')
        search(i, j);
    }
  }
}

int
main(void)
{
  char CRLF;
  for (int i = 0; i < BOARD_ROWS; ++i)
  {
    for (int j = 0; j < BOARD_COLS; ++j)
    {
      scanf("%c", &board[i][j]);
      if(board[i][j] == 'X')
        x_cnt++;
      else if(board[i][j] == 'O')
        o_cnt++;
    }
    // 存储换行符
    scanf("%c", &CRLF);
  }
  x_next = x_cnt > o_cnt ? false : true;
  traverse();
  if (!already_win_flag && win_x != 16)
  {
    printf("Win!\n");
    printf("%d %d\n", win_x + 1, win_y + 1);
  }
  else if (!already_win_flag)
    printf("CannotWin!\n");

  return 0;
}
