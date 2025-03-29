#pragma once

/// \file
/// \brief This file introduces a renderer which
/// helps us visualize objects in the scene.

//
//
//
//
//
#include "Base.h"
#include "Scene.h"

/// \brief Let the renderer automatically deduce the color.
#define TK_AUTO_COLOR ((const char *)(NULL))

/// \brief The error message for undefined behaviors.
#define TK_INVALID_COLOR "Invalid Color"

/// \example Consequently call these functions to render a frame.
/// ```c
/// RdrClear();
/// // Update objects in the scene here.
/// RdrRender();
/// RdrFlush();
/// ```
///
/// \details The renderer is implemented by
/// imitating the double buffering technique.
/// See https://en.wikipedia.org/wiki/Multiple_buffering for more details.
typedef struct {
  char *csPrev;      // Characters of the previous frame.
  Color *colorsPrev; // Character colors of the previous frame.
  char *cs;          // Characters of the current frame.
  Color *colors;     // Character colors of the current frame.
} Renderer;

// The renderer singleton.
static Renderer renderer;


// 上下左右(东南西北)的全局相对Vec(相对坐标)
static Vec vecNN = {-1, -1};
static Vec vecPP = {+1, +1};
static Vec vecNP = {-1, +1};
static Vec vecPN = {+1, -1};
static Vec vecOP = {0, +1};
static Vec vecON = {0, -1};
static Vec vecPO = {+1, 0};
static Vec vecNO = {-1, 0};
//
//
//
/// \brief Render character `c` at position `pos` with color `color`.
///
/// \example ```c
/// // Explicitly specify the color.
/// RdrPutChar(pos, 'o', TK_RED);
/// // Let the renderer automatically deduce the color.
/// RdrPutChar(pos, eFlagWall, TK_AUTO_COLOR);
/// ```
void RdrPutChar(Vec pos, char c, Color color) {
  renderer.cs[Idx(pos)] = c;

  if (color == TK_AUTO_COLOR) {
    Flag flag = (Flag)c;
    color = flag == eFlagNone    ? TK_NORMAL
            : flag == eFlagSolid ? TK_BLUE
            : flag == eFlagWall  ? TK_WHITE

            : flag == eFlagTank ? TK_INVALID_COLOR
                                : TK_INVALID_COLOR;
  }
  renderer.colors[Idx(pos)] = color;
}

//
//
//
/// \brief Clear all the objects in the scene from the frame.
void RdrClear(void) {
  // Clear tanks.
  for (RegIterator it = RegBegin(regTank); it != RegEnd(regTank); it = RegNext(it)) {
    Tank *tank = RegEntry(regTank, it);
    Vec pos = tank->pos;

    for (int y = -1; y <= 1; ++y)
      for (int x = -1; x <= 1; ++x)
        RdrPutChar(Add(pos, (Vec){x, y}), map.flags[Idx(pos)], TK_AUTO_COLOR);
  }

  // Clear bullets.
  for (RegIterator it = RegBegin(regBullet); it != RegEnd(regBullet); it = RegNext(it)) {
    Bullet *bullet = RegEntry(regBullet, it);
    Vec pos = bullet->pos;

    RdrPutChar(pos, map.flags[Idx(pos)], TK_AUTO_COLOR);
  }
}

/// \brief Render all the objects in the scene to the frame.
void RdrRender(void) {
  // Render tanks.
  // 先渲染tank
  for (RegIterator it = RegBegin(regTank); it != RegEnd(regTank); it = RegNext(it)) {
    Tank *tank = RegEntry(regTank, it);
    Vec pos = tank->pos;
    Vec vecHead, vecTail, vecLhand, vecRhand;
    Vec relative_pos = {0, 0};
    char direction_char = 'E';
    Color color = tank->color;

    // 标记上下左右
    switch (tank->dir) {
      default:
        vecHead = vecPO; vecTail = vecNO; vecLhand = vecON; vecRhand = vecOP;
        break;
      case eDirNN:
        vecHead = vecNN; vecTail = vecPP; vecLhand = vecNP; vecRhand = vecPN;
        break;
      case eDirNO:
        vecHead = vecNO; vecTail = vecPO; vecLhand = vecON; vecRhand = vecOP;
        break;
      case eDirNP:
        vecHead = vecNP; vecTail = vecPN; vecLhand = vecPP; vecRhand = vecNN;
        break;
      case eDirON:
        vecHead = vecON; vecTail = vecOP; vecLhand = vecPO; vecRhand = vecNO;
        break;
      case eDirOP:
        vecHead = vecOP; vecTail = vecON; vecLhand = vecNO; vecRhand = vecPO;
        break;
      case eDirPN:
        vecHead = vecPN; vecTail = vecNP; vecLhand = vecNN; vecRhand = vecPP;
        break;
      case eDirPO:
        vecHead = vecPO; vecTail = vecNO; vecLhand = vecOP; vecRhand = vecON; 
        break;
      case eDirPP:
        vecHead = vecPP; vecTail = vecNN; vecLhand = vecPN; vecRhand = vecNP;
        break;
    }
    for (int y = -1; y <= 1; ++y) {
      for (int x = -1; x <= 1; ++x) {
        relative_pos.x = x; relative_pos.y = y;
        direction_char = 'O';
        if (Eq(relative_pos, vecHead)) {direction_char = 'N';}
        if (Eq(relative_pos, vecTail)) {direction_char = 'S';}
        if (Eq(relative_pos, vecLhand))  {direction_char = 'E';}
        if (Eq(relative_pos, vecRhand))  {direction_char = 'W';}
        RdrPutChar(Add(pos, relative_pos), direction_char, color);
      }
    }
  }

  // Render bullets.
  for (RegIterator it = RegBegin(regBullet); it != RegEnd(regBullet); it = RegNext(it)) {
    Bullet *bullet = RegEntry(regBullet, it);
    Vec pos = bullet->pos;
    Color color = bullet->color;

    RdrPutChar(pos, 'o', color);
  }
}

/// \brief Flush the previously rendered frame to screen to
/// make it truly visible.
void RdrFlush(void) {
  char *csPrev = renderer.csPrev;
  Color *colorsPrev = renderer.colorsPrev;
  const char *cs = renderer.cs;
  const Color *colors = renderer.colors;

  // 使用double buffering避免重复渲染没有变动的对象
  for (int y = 0; y < map.size.y; ++y) {
    for (int x = 0; x < map.size.x; ++x) {
      Vec pos = {x, y};
      int i = Idx(pos);

      if (cs[i] != csPrev[i] || colors[i] != colorsPrev[i]) {
        MoveCursor(pos);
        printf(TK_TEXT("%c", TK_RUNTIME_COLOR), colors[i], cs[i]);
        // 更新cs和colors
        csPrev[i] = cs[i];
        colorsPrev[i] = colors[i];
      }
    }
  }
}
