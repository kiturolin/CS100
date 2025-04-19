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
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Base.h"
#include "Config.h"
#include "Registry.h"
#include "Renderer.h"
#include "Scene.h"

#define CLOCKWISE            1
#define ANTICLOCKWISE        0
#define FORWARD              1
#define BACKWARD             0
#define BLOCK_POS_VALIDITY   3
#define DEFAULT_POS_VALIDITY 2

typedef struct {
  char keyHit; // The keyboard key hit by the player at this frame.
} Game;

typedef enum {
  eViolationTypeNone,
  eViolationTypeBorder,
  eViolationTypeTank,
  eViolationTypeSolid,
  eViolationTypeWall
} ViolationType;

// The game singleton.
static Game game;

// The keyboard key "ESC".
static const char keyESC = '\033';

static uint64_t frame;

Vec vecImpossible = {INT16_MAX, INT16_MAX};

Vec
UtilDirection2Vec (const Dir *dir);
ViolationType
UtilPosViolation (const Vec *pos, Vec ignoredBlock);
ViolationType
UtilBlockViolation (const Vec *block_pos, Vec ignoredBlock);
void
TankTurn (Tank *tank, bool direction);
void
TankShoot (Tank *tank);
void
TankPlayerTankUpdate (Tank *tank, char key);
void
BulletMove (Bullet *bullet);
void
BulletHit (Bullet *bullet);
void
ScenePlaceWall (Block *block);
void
SceneGenerateRandomBlock (void);
void
SceneGenerateEnemy (void);

// 放置可破坏的墙体
void
ScenePlaceWall (Block *block)
{
  Vec particle;
  for (int y = -1; y <= 1; ++y) {
    for (int x = -1; x <= 1; ++x) {
      particle = Add (block->pos, (Vec){x, y});
      if (!Eq (particle, block->pos)) {
        Block *block_wall = RegNew (regBlock);
        block_wall->pos = particle;
        block_wall->type = eBlockTypeWall;
      }
    }
  }
}

void
SceneGenerateRandomBlock (void)
{
  Vec vecBlock;
  for (int i = 0; i < config.nSolids + config.nWalls;) {
    vecBlock = RandPos ();
    if (UtilBlockViolation (&vecBlock, vecImpossible) == eViolationTypeNone) {
      Block *block = RegNew (regBlock);
      block->pos = vecBlock;
      block->type = i < config.nSolids ? eBlockTypeSolid : eBlockTypeWall;
      if (block->type == eBlockTypeWall) { ScenePlaceWall (block); }
      ++i;
    }
  }
}

void
SceneSearchDeleteWall (const Vec wall_pos)
{
  for (RegIterator it = RegBegin (regBlock); it != RegEnd (regBlock);) {
    Block *block = RegEntry (regBlock, it);
    it = RegNext (it);
    if (Eq (block->pos, wall_pos) && block->type == eBlockTypeWall) {
      RegDelete (block);
      RdrPutChar (block->pos, ' ', TK_NORMAL);
    }
  }
}

void
SceneGenerateEnemy (void)
{
  Vec vecTank;
  for (int i = 0; i < config.nEnemies;) {
    vecTank = RandPos ();
    if (UtilBlockViolation (&vecTank, vecImpossible) == eViolationTypeNone) {
      Tank *tank = RegNew (regTank);
      tank->pos = vecTank;
      tank->color = TK_RED;
      tank->dir = eDirOP;
      tank->hp = 5;
      tank->isPlayer = false;
      tank->last_shoot = 0;
      tank->last_action = 0;
      ++i;
    }
  }
}

/// \brief Check if pos violated existing objects(Tank, Wall, Solid, Border)
///         and return violation type;
ViolationType
UtilPosViolation (const Vec *pos, const Vec ignoredBlock)
{
  bool border_flag = pos->x <= map.size.x - 2 && pos->x >= 1 && pos->y <= map.size.y - 2 && pos->y >= 1;

  if (!border_flag) { return eViolationTypeBorder; }

  Vec relative_pos;
  // 监测是否与tank有violation
  for (RegIterator itT = RegBegin (regTank); itT != RegEnd (regTank); itT = RegNext (itT)) {
    Tank *tank = RegEntry (regTank, itT);
    if (Eq (tank->pos, ignoredBlock)) { continue; }
    relative_pos = Sub (tank->pos, *pos);
    if (abs (relative_pos.x) <= 1 && abs (relative_pos.y) <= 1) { return eViolationTypeTank; }
  }
  // 监测是否与block有violation
  for (RegIterator itS = RegBegin (regBlock); itS != RegEnd (regBlock); itS = RegNext (itS)) {
    Block *block = RegEntry (regBlock, itS);
    if (Eq (block->pos, ignoredBlock)) { continue; }
    relative_pos = Sub (block->pos, *pos);
    if (abs (relative_pos.x) <= 1 && abs (relative_pos.y) <= 1 && block->type == eBlockTypeSolid) {
      return eViolationTypeSolid;
    }
    if (Eq (*pos, block->pos) && block->type == eBlockTypeWall) { return eViolationTypeWall; }
  }
  return eViolationTypeNone;
}

// 监测block的位置是否与其他对象重叠: tank, wall, solid或border
ViolationType
UtilBlockViolation (const Vec *block_pos, const Vec ignoredBlock)
{
  ViolationType violation = eViolationTypeNone;
  Vec particle;
  // 检查八个方位是否有重叠
  for (int y = -1; y <= 1; ++y) {
    for (int x = -1; x <= 1; ++x) {
      particle = Add (*block_pos, (Vec){x, y});
      violation = UtilPosViolation (&particle, ignoredBlock);
      if (violation != eViolationTypeNone) { return violation; }
    }
  }

  return eViolationTypeNone;
}

/// \brief Transform dir to relative Vec
Vec
UtilDirection2Vec (const Dir *dir)
{
  Vec vecForward;
  switch (*dir) {
  default:
    vecForward = vecNN;
    break;
  case eDirNN:
    vecForward = vecNN;
    break;
  case eDirNO:
    vecForward = vecNO;
    break;
  case eDirNP:
    vecForward = vecNP;
    break;
  case eDirON:
    vecForward = vecON;
    break;
  case eDirOP:
    vecForward = vecOP;
    break;
  case eDirPN:
    vecForward = vecPN;
    break;
  case eDirPO:
    vecForward = vecPO;
    break;
  case eDirPP:
    vecForward = vecPP;
    break;
  }
  return vecForward;
}

inline void
TankTurn (Tank *pTank, bool direction)
{
  pTank->dir = direction ? ((pTank->dir + 1) % eDirInvalid) : ((pTank->dir - 1 + eDirInvalid) % eDirInvalid);
  // 如果转到了Center位置, 就再转一次
  if (pTank->dir == eDirOO) { TankTurn (pTank, direction); }
}

void
TankMove (Tank *tank, bool direction)
{
  Vec vecForward = UtilDirection2Vec (&tank->dir);
  Vec tmp_pos = direction ? Add (tank->pos, vecForward) : Sub (tank->pos, vecForward);
  tank->pos = UtilBlockViolation (&tmp_pos, tank->pos) == eViolationTypeNone ? tmp_pos : tank->pos;
}

// 子弹向前移动一格
void
BulletMove (Bullet *bullet)
{
  Vec vecForward = UtilDirection2Vec (&bullet->dir);
  Vec tmp_pos = Add (bullet->pos, vecForward);
  bullet->pos = tmp_pos;
}

/// \brief Check if the bullet hits wall or another tank.
/// 
// 监测子弹是否打中了某个坦克或某个Wall
void
BulletHit (Bullet *bullet)
{
  ViolationType bullet_violation = UtilPosViolation (&bullet->pos, vecImpossible);
  if (bullet_violation == eViolationTypeBorder || bullet_violation == eViolationTypeSolid) {
    bullet->removed = true;
    return;
  }

  if (bullet_violation == eViolationTypeWall) {
    SceneSearchDeleteWall (bullet->pos);
    bullet->removed = true;
    return;
  }
  Vec relative_pos;
  for (RegIterator itT = RegBegin (regTank); itT != RegEnd (regTank);) {
    Tank *tank = RegEntry (regTank, itT);
    itT = RegNext (itT);
    relative_pos = Sub (tank->pos, bullet->pos);
    // 如果子弹打中了敌方坦克
    if (abs (relative_pos.x) <= 1 && abs (relative_pos.y) <= 1) {
      if (--tank->hp == 0) { RegDelete (tank); }
      bullet->removed = true;
    }
  }
}

void
TankShoot (Tank *tank)
{
  if (frame - tank->last_shoot > 15) {
    Bullet *bullet = RegNew (regBullet);
    Vec bulletRelativeVec = UtilDirection2Vec (&tank->dir);
    bullet->dir = tank->dir;
    bullet->pos = Add (tank->pos, bulletRelativeVec);
    bullet->isPlayer = tank->isPlayer;
    bullet->color = tank->color;
    bullet->removed = false;

    tank->last_shoot = frame;
  }
}

/// \brief Update Player's tank state.
/// 
void
TankPlayerTankUpdate (Tank *tank, char key)
{
  switch (key) {
  case 'a':
    TankTurn (tank, CLOCKWISE);
    break;
  case 'd':
    TankTurn (tank, ANTICLOCKWISE);
    break;
  case 'w':
    TankMove (tank, FORWARD);
    break;
  case 's':
    TankMove (tank, BACKWARD);
    break;
  case 'k':
    TankShoot (tank);
    break;
  default:;
  }
}

//
//
//
/// \brief Configure the scene (See `Scene.h`) with `config` (See `Config.h`),
/// and perform initializations including:
/// 1. Terminal setup.
/// 2. Memory allocations.
/// 3. Map and object generations.
/// 4. Rendering of the initialized scene.
///
/// \note This function should be called at the very beginning of
/// `GameLifecycle`.
void
GameInit (void)
{
  // Setup terminal.
  TermSetupGameEnvironment ();
  TermClearScreen ();

  // Configure scene.
  map.size = config.mapSize;
  int nEnemies = config.nEnemies;
  int nSolids = config.nSolids;
  int nWalls = config.nWalls;

  // Initialize scene.
  RegInit (regTank);
  RegInit (regBullet);
  RegInit (regBlock);

  map.flags = (Flag *)malloc (sizeof (Flag) * map.size.x * map.size.y);
  // 初始化地图
  for (int y = 0; y < map.size.y; ++y) {
    for (int x = 0; x < map.size.x; ++x) {
      Vec pos = {x, y};

      // 默认将此坐标初始化为空None
      Flag flag = eFlagNone;
      // 将边界设置为不可穿过的墙
      if (x == 0 || y == 0 || x == map.size.x - 1 || y == map.size.y - 1) { flag = eFlagSolid; }

      map.flags[Idx (pos)] = flag;
    }
  }

  // 初始化玩家tank
  {
    Tank *tank = RegNew (regTank);
    // 默认出生点为(2, 2)
    tank->pos = (Vec){2, 2};
    // 默认方向朝上
    tank->dir = eDirOP;
    tank->color = TK_GREEN;
    tank->isPlayer = true;
    tank->hp = 5;
    tank->last_shoot = 0;
    tank->last_action = 0;
  }

  // Initialize renderer.
  // 初始化renderer, 渲染器(renderer)实质上是四个字符数组, 每个数组的大小为map
  // size 使用double buffering技术, 此处先为四个数组分配内存空间
  renderer.csPrev = (char *)malloc (sizeof (char) * map.size.x * map.size.y);
  renderer.colorsPrev = (Color *)malloc (sizeof (Color) * map.size.x * map.size.y);
  renderer.cs = (char *)malloc (sizeof (char) * map.size.x * map.size.y);
  renderer.colors = (Color *)malloc (sizeof (Color) * map.size.x * map.size.y);

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
      RdrPutChar (pos, map.flags[Idx (pos)], TK_AUTO_COLOR);
    }
  }

  SceneGenerateRandomBlock ();
  SceneGenerateEnemy ();

  RdrRender ();
  RdrFlush ();
}

//
//
//
/// \brief Read input from the player.
///
/// \note This function should be called in the loop of `GameLifecycle` before
/// `GameUpdate`.
void
GameInput (void)
{
  game.keyHit = kbhit_t () ? (char)getch_t () : '\0';
}

//
//
//
/// \brief Perform all tasks required for a frame update, including:
/// 1. Game logics of `Tank`s, `Bullet`s, etc.
/// 2. Rerendering all objects in the scene.
///
/// \note This function should be called in the loop of `GameLifecycle` after
/// `GameInput`.
void
GameUpdate (void)
{
  RdrClear ();

  for (RegIterator it = RegBegin (regTank); it != RegEnd (regTank); it = RegNext (it)) {
    Tank *tank = RegEntry (regTank, it);

    if (tank->isPlayer) {
      TankPlayerTankUpdate (tank, game.keyHit);
    } else {
      if (frame - tank->last_action > 15){
        int8_t action = rand () % 3;
        if (action == 0) { TankMove (tank, FORWARD); }
        if (action == 1) { TankTurn (tank, ANTICLOCKWISE); }
        tank->last_action = frame;
      }
    }
  }

  for (RegIterator it = RegBegin (regBullet); it != RegEnd (regBullet);) {
    Bullet *bullet = RegEntry (regBullet, it);
    it = RegNext (it);
    BulletMove (bullet);
    BulletHit (bullet);
    if (bullet->removed) { RegDelete (bullet); }
  }

  RdrRender ();
  RdrFlush ();
}

//
//
//
/// \brief Terminate the game and free all the resources.
///
/// \note This function should be called at the very end of `GameLifecycle`.
void
GameTerminate (void)
{
  while (RegSize (regTank) > 0) RegDelete (RegEntry (regTank, RegBegin (regTank)));

  while (RegSize (regBullet) > 0) RegDelete (RegEntry (regBullet, RegBegin (regBullet)));

  while (RegSize (regBlock) > 0) RegDelete (RegEntry (regBlock, RegBegin (regBlock)));

  free (map.flags);
  free (renderer.csPrev);
  free (renderer.colorsPrev);
  free (renderer.cs);
  free (renderer.colors);

  TermClearScreen ();
}

//
//
//
/// \brief Lifecycle of the game, defined by calling the 4 important functions:
/// `GameInit`, `GameInput`, `GameUpdate`, and `GameTerminate`.
///
/// \note This function should be called by `main`.
void
GameLifecycle (void)
{
  GameInit ();

  // 每帧时间, 单位为ms
  double frameTime = (double)1000 / (double)config.fps;
  clock_t frameBegin = clock ();

  while (true) {
    GameInput ();
    if (game.keyHit == keyESC) break;

    frame++;
    GameUpdate ();

    while (((double)(clock () - frameBegin) / CLOCKS_PER_SEC) * 1000.0 < frameTime - 0.5) Daze ();
    frameBegin = clock ();
  }

  GameTerminate ();
}
