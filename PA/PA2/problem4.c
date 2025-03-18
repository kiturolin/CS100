#include <stdio.h>
#include <stdint.h>

#define MAX_WORLDLINE 1000
#define MAX_EVENTS 4000

// p[0]和s[0]我们都不用
uint32_t n;
uint32_t p[MAX_WORLDLINE + 1];
uint32_t s[MAX_WORLDLINE + 1];
uint32_t changed[MAX_WORLDLINE + 1];

// 返回下一条世界线的编号
// 如果下一条世界线形成回环(指向自身)或其sigularity value被修改, 则返回-1
int
next_line(uint32_t prev, uint32_t start)
{
  int next = -1;
  next = p[prev];
  if ((uint32_t)next == prev || (uint32_t)next == start ||(changed[next] == 1))
    return -1;
  return next;
}

void
recalc_s(uint32_t x, uint32_t s1, uint32_t cnt)
{
  uint32_t s0 = s[x];
  cnt = cnt % 32;
  s0 = cnt == 0 ? s0 ^ s1 : s0 ^ ((s1 << cnt) | (s1 >> (32 - cnt)));
  s[x] = s0;
  changed[x] = 1;
}

void 
refresh_s()
{
  for (uint32_t i = 1; i < n + 1; ++i)
    changed[i] = 0;
}

void
update(uint32_t x, uint32_t y)
{
  uint32_t s_x_initial = s[x];
  int32_t next, cnt = 0;
  p[x] = y;
  next = x;
  while((next = next_line(next, x)) != -1)
    recalc_s(next, s_x_initial, ++cnt);
}

int 
main(void)
{
  uint32_t Q;
  uint32_t min_s = UINT32_MAX;
  uint32_t min_i = UINT32_MAX;
  scanf("%u", &n);
  scanf("%u", &Q);

  for (uint32_t i = 1; i < n + 1; ++i)
  {
    scanf("%u", &s[i]);
    // 初始化pi
    p[i] = i;
  }
  refresh_s();

  for (uint32_t i = 0; i < Q; ++i)
  {
    uint32_t x, y;
    scanf("%u", &x);
    scanf("%u", &y);
    update(x, y);
    refresh_s();
  }

  for (uint32_t i = 1; i < n + 1; ++i)
    if (s[i] < min_s){min_s = s[i]; min_i = i;}
  
  printf("%u\n", min_i);
  printf("%u\n", min_s);
  // printf("p:\n");
  // for(uint32_t i = 1; i < n + 1; ++i) printf("%d ", p[i]);
  // printf("\ns:\n");
  // for(uint32_t i = 1; i < n + 1; ++i) printf("%d ", s[i]);
  return 0;
}
