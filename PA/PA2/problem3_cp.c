#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#define BOARD_COLS 15
#define BOARD_ROWS 15

char board[BOARD_ROWS][BOARD_COLS];
uint32_t win_x = 16;
uint32_t win_y = 16;
uint32_t x_cnt;
uint32_t o_cnt;
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
  return (x <= BOARD_ROWS) && (y <= BOARD_COLS);
}

// 返回值为是否进行下一次forward或backward搜索
bool
check(uint8_t x, uint8_t y, char c, uint8_t *pieces_cnt)
{
  char c1 = board[x][y];
  if (c1 == c && *pieces_cnt == 4)
  {
    printf("AlreadyWin!\n");
    already_win_flag = true;
    return false;
  } 
  else if (c1 == '.' && *pieces_cnt == 4)
  {
    if ((x_next && c == 'X') || (!x_next && c == 'O'))
    {
      win_x = x;
      win_y = y;
    }
    return false;
  }
  else if (c1 == c)
  {
    (*pieces_cnt)++;
    return true;
  }
  // 碰壁的情况
  else
  {
    return false;
  }

}

void
line_search(uint8_t x, uint8_t y, uint8_t direction)
{
  uint8_t pieces_cnt = 1;
  char c = board[x][y];
  switch (direction) {
    case HORIZON:
      for (int i = 1; i < 5; ++i)
      {
        if (!in_board(x, y + i) || !check(x, y + i, c, &pieces_cnt))
          break;
      }
      for (int i = 1; i < 5; ++i)
      {
        if (!in_board(x, y - i) ||!check(x, y - i, c,  &pieces_cnt))
          break;
      }
      break;
    case VERTICAL:
      for (int i = 1; i < 5; ++i)
      {
        if (!in_board(x + i, y) || !check(x + i, y, c, &pieces_cnt))
          break;
      }
      for (int i = 1; i < 5; ++i)
      {
        if (!in_board(x - i, y) || !check(x - i, y, c, &pieces_cnt))
          break;
      }
      break;
    case LDIAG:
      for (int i = 1; i < 5; ++i)
      {
        if (!in_board(x - i, y - i) || !check(x - i, y - i, c, &pieces_cnt))
          break;
      }
      for (int i = 1; i < 5; ++i)
      {
        if (!in_board(x + i, y + i) || !check(x + i, y + i, c, &pieces_cnt))
          break;
      }
      break;
    case RDIAG:
      for (int i = 1; i < 5; ++i)
      {
        if (!in_board(x - i, y + i) || !check(x - i, y + i, c, &pieces_cnt))
          break;
      }
      for (int i = 1; i < 5; ++i)
      {
        if (!in_board(x + i, y - i) || !check(x + i, y - i, c, &pieces_cnt))
          break;
      }
      break;
    }
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
    printf("CannotWin\n");

  return 0;
}
