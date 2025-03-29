#pragma once

/// \file
/// \brief This file contains the game lifecycle and logics.
/// There are 5 important functions:
/// `GameInit`, `GameInput`, `GameUpdate`, `GameTerminate`, and
/// the most important: `GameLifecycle`.
/// Please read the corresponding comments to understand their relationships.

//
//
//
//
//
#include "Base.h"
#include "Config.h"
#include "Renderer.h"
#include "Scene.h"

#include <time.h>

#define CLOCKWISE 1 
#define ANTICLOCKWISE 0
#define FORWARD 1 
#define BACKWARD 0
typedef struct {
  char keyHit; // The keyboard key hit by the player at this frame.
} Game;

// The game singleton.
static Game game;

// The keyboard key "ESC".
static const char keyESC = '\033';

bool TankPosValidity(Vec *pos);
void TankTurn(Tank *tank, bool direction);
void TankPlayerTankUpdate(Tank *tank, char key);

inline bool 
TankPosValidity(Vec *pos)
{
  return  pos->x <= map.size.x - 3 && 
          pos->x >= 2 &&
          pos->y <= map.size.y - 3 &&
          pos->y >= 2;
}

inline void 
TankTurn(Tank *pTank, bool direction)
{
  pTank->dir = direction ? ((pTank->dir + 1) % eDirInvalid) : ((pTank->dir - 1 + eDirInvalid) % eDirInvalid);
  // 如果转到了Center位置, 就再转一次
  if(pTank->dir == eDirOO) {TankTurn(pTank, direction);}
}

void TankMove(Tank *tank, bool direction)
{
  Vec vecForward;
  switch (tank->dir) {
    default:
      vecForward = vecNN; break;
    case eDirNN:
      vecForward = vecNN; break;
    case eDirNO:
      vecForward = vecNO; break;
    case eDirNP:
      vecForward = vecNP; break;
    case eDirON:
      vecForward = vecON; break;
    case eDirOP:
      vecForward = vecOP; break;
    case eDirPN:
      vecForward = vecPN; break;
    case eDirPO:
      vecForward = vecPO; break;
    case eDirPP:
      vecForward = vecPP; break;
  }
 
  Vec tmp_pos = direction ? Add(tank->pos, vecForward) : Sub(tank->pos, vecForward); 
  tank->pos = TankPosValidity(&tmp_pos) ? tmp_pos : tank->pos;
}


void
TankPlayerTankUpdate(Tank *tank, char key){
  switch (key) {
    case 'a':
      TankTurn(tank, CLOCKWISE);
      break;
    case 'd':
      TankTurn(tank, ANTICLOCKWISE);
      break;
    case 'w':
      TankMove(tank, FORWARD);
      break;
    case 's':
      TankMove(tank, BACKWARD);
    default:
      ;
  }
}
//
//
//
/// \brief Configure the scene (See `Scene.h`) with `config` (See `Config.h`), and
/// perform initializations including:
/// 1. Terminal setup.
/// 2. Memory allocations.
/// 3. Map and object generations.
/// 4. Rendering of the initialized scene.
///
/// \note This function should be called at the very beginning of `GameLifecycle`.
void GameInit(void) {
  // Setup terminal.
  TermSetupGameEnvironment();
  TermClearScreen();

  // Configure scene.
  map.size = config.mapSize;
  int nEnemies = config.nEnemies;
  int nSolids = config.nSolids;
  int nWalls = config.nWalls;

  // Initialize scene.
  RegInit(regTank);
  RegInit(regBullet);

  map.flags = (Flag *)malloc(sizeof(Flag) * map.size.x * map.size.y);
  // 初始化地图
  for (int y = 0; y < map.size.y; ++y) {
    for (int x = 0; x < map.size.x; ++x) {
      Vec pos = {x, y};

      // 默认将此坐标初始化为空None
      Flag flag = eFlagNone;
      // 将边界设置为不可穿过的墙
      if (x == 0 || y == 0 || x == map.size.x - 1 || y == map.size.y - 1) {
        flag = eFlagSolid;
      }

      map.flags[Idx(pos)] = flag;
    }
  }

  // 初始化玩家tank
  {
    Tank *tank = RegNew(regTank);
    // 默认出生点为(2, 2)
    tank->pos = (Vec){2, 2};
    // 默认方向朝上
    tank->dir = eDirOP;
    tank->color = TK_GREEN;
    tank->isPlayer = true;
  }

  // Initialize renderer.
  // 初始化renderer, 渲染器(renderer)实质上是四个字符数组, 每个数组的大小为map size 
  // 使用double buffering技术, 此处先为四个数组分配内存空间
  renderer.csPrev = (char *)malloc(sizeof(char) * map.size.x * map.size.y);
  renderer.colorsPrev = (Color *)malloc(sizeof(Color) * map.size.x * map.size.y);
  renderer.cs = (char *)malloc(sizeof(char) * map.size.x * map.size.y);
  renderer.colors = (Color *)malloc(sizeof(Color) * map.size.x * map.size.y);

  // 初始化渲染器内的"前一帧", 因为malloc不会零初始化内存
  for (int i = 0; i < map.size.x * map.size.y; ++i) {
    renderer.csPrev[i] = renderer.cs[i] = ' ';
    renderer.colorsPrev[i] = renderer.colors[i] = TK_NORMAL;
  }

  // Render scene.
  // 使用二重for循环来渲染场景
  for (int y = 0; y < map.size.y; ++y) {
    for (int x = 0; x < map.size.x; ++x) {
      Vec pos = {x, y};
      // 在Vec坐标上放置字符c, 并设置其颜色为TK_AUTO_COLOR
      RdrPutChar(pos, map.flags[Idx(pos)], TK_AUTO_COLOR);
    }
  }
  RdrRender();
  RdrFlush();
}

//
//
//
/// \brief Read input from the player.
///
/// \note This function should be called in the loop of `GameLifecycle` before `GameUpdate`.
void GameInput(void) {
  game.keyHit = kbhit_t() ? (char)getch_t() : '\0';
}

//
//
//
/// \brief Perform all tasks required for a frame update, including:
/// 1. Game logics of `Tank`s, `Bullet`s, etc.
/// 2. Rerendering all objects in the scene.
///
/// \note This function should be called in the loop of `GameLifecycle` after `GameInput`.
void GameUpdate(void) {
  RdrClear();

  // TODO(kituro): You may need to delete or add codes here.
  for (RegIterator it = RegBegin(regTank); it != RegEnd(regTank); it = RegNext(it)) {
    Tank *tank = RegEntry(regTank, it);
    
    if (tank->isPlayer)
      {TankPlayerTankUpdate(tank, game.keyHit);}
  }

  RdrRender();
  RdrFlush();
}

//
//
//
/// \brief Terminate the game and free all the resources.
///
/// \note This function should be called at the very end of `GameLifecycle`.
void GameTerminate(void) {
  while (RegSize(regTank) > 0)
    RegDelete(RegEntry(regTank, RegBegin(regTank)));

  while (RegSize(regBullet) > 0)
    RegDelete(RegEntry(regBullet, RegBegin(regBullet)));

  free(map.flags);

  free(renderer.csPrev);
  free(renderer.colorsPrev);
  free(renderer.cs);
  free(renderer.colors);

  TermClearScreen();
}

//
//
//
/// \brief Lifecycle of the game, defined by calling the 4 important functions:
/// `GameInit`, `GameInput`, `GameUpdate`, and `GameTerminate`.
///
/// \note This function should be called by `main`.
void GameLifecycle(void) {
  GameInit();

  // 每帧时间, 单位为ms
  double frameTime = (double)1000 / (double)config.fps;
  clock_t frameBegin = clock();

  while (true) {
    GameInput();
    if (game.keyHit == keyESC)
      break;

    GameUpdate();

    while (((double)(clock() - frameBegin) / CLOCKS_PER_SEC) * 1000.0 < frameTime - 0.5)
      Daze();
    frameBegin = clock();
  }

  GameTerminate();
}
