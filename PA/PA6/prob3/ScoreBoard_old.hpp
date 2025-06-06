#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "ArtDigits.hpp"
#include "utils.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

inline constexpr auto MSPerTick = 50;

class ScoreBoard
{
  enum class PlayerSide { Left, Right };

  const std::string mNames[2];
  int mScores[2];

  enum class ScrollDirection { Up, Down };

  template <std::size_t Height, std::size_t Width>
  static void printBlock (const char (&object)[Height][Width], int baseRow, int baseCol)
  {
    for (int row = 0; row != Height; ++row) {
      move_cursor (baseRow + row, baseCol);
      std::cout << object[row];
    }
    std::cout << std::flush;
  }

  static void scrollDigit (ScrollDirection dir, int oldDigit, int newDigit, int baseRow, int baseCol)
  {
    // 绘图的坐标原点在左上角!
    // 如果dir为Up, 那么scroll后的新数字为bottomDigit
    // 如果dir为Down, 那么scroll后的新数字为topDigit
    int topDigit    = dir == ScrollDirection::Up ? oldDigit : newDigit;
    // 如果topDigit为newDigit, 那么bottomDigit为oldDigit, 反之亦然
    int bottomDigit = oldDigit + newDigit - topDigit;
    // 实现翻页的酷炫动画
    // 最外层的for循环每次打印一帧
    for (int tick = 0; tick != DigitHeight + 1; ++tick) {
      int mid = dir == ScrollDirection::Up ? tick : DigitHeight - tick;
      // 每次从Pad的最上方一行开始打印
      int row = baseRow;
      // 打印一行
      // 两个循环加在一起运行的次数就是DigitalHeight
      for (int i = mid; i != DigitHeight; ++i) {
        move_cursor (row++, baseCol);
        std::cout << ArtDigit[topDigit][i];
      }
      for (int i = 0; i != mid; ++i) {
        move_cursor (row++, baseCol);
        std::cout << ArtDigit[bottomDigit][i];
      }
      std::cout << std::flush;
      sleep_ms (MSPerTick);
    }
  }

  static TextColor ColorOnSide (PlayerSide side) { return side == PlayerSide::Left ? TextColor::Red : TextColor::Blue; }

  void updateScore (PlayerSide player, int delta)
  {
    int &score	 = mScores[static_cast<int> (player)];
    int newScore = score + delta;
    assert (newScore >= 0 && newScore < 100);

    int baseCol = DigitBaseCol;
    if (player == PlayerSide::Right) baseCol += FrameWidth * 2 + ColonWidth;

    // 保护我方打印object的程序
    PrinterGuard printerGuard;
    // 将默认打印颜色设置为选手对应的颜色
    ColorGuard withColor (ColorOnSide (player));

    auto dir = delta > 0 ? ScrollDirection::Up : ScrollDirection::Down;
    scrollDigit (dir, score % 10, newScore % 10, DigitBaseRow, baseCol + FrameWidth);
    if (score / 10 != newScore / 10) scrollDigit (dir, score / 10, newScore / 10, DigitBaseRow, baseCol);

    score += delta;
  }

public:
  ScoreBoard (std::string leftName, std::string rightName) :
      mNames{ { std::move (leftName) }, { std::move (rightName) } },
      mScores{}
  {
    clear_screen ();

    PrinterGuard printerGuard;

    std::cout << TextColor::Red;
    // 左侧选手十位数框
    printBlock (Frame, 0, 0);
    // 左侧选手个位数框
    printBlock (Frame, 0, FrameWidth);
    // 左侧选手的分数
    printBlock (ArtDigit[0], DigitBaseRow, DigitBaseCol);
    printBlock (ArtDigit[0], DigitBaseRow, FrameWidth + DigitBaseCol);
    std::cout << TextColor::Blue;
    // 右侧选手十位数框
    printBlock (Frame, 0, FrameWidth * 2 + ColonWidth);
    // 右侧选手个位数框
    printBlock (Frame, 0, FrameWidth * 3 + ColonWidth);
    // 右侧选手分数
    printBlock (ArtDigit[0], DigitBaseRow, FrameWidth * 2 + ColonWidth + DigitBaseCol);
    printBlock (ArtDigit[0], DigitBaseRow, FrameWidth * 3 + ColonWidth + DigitBaseCol);

    std::cout << TextColor::Normal;

    // 中间的分隔符: 冒号
    printBlock (Colon, 0, FrameWidth * 2);

    // 输出名字 注意名字居中
    move_cursor (FrameHeight, FrameWidth - mNames[0].size () / 2);
    std::cout << mNames[0];

    move_cursor (FrameHeight, 3 * FrameWidth + ColonWidth - mNames[1].size () / 2);
    std::cout << mNames[1];

    std::cout << std::flush;
  }

  void leftInc (int delta = 1) { updateScore (PlayerSide::Left, delta); }

  void rightInc (int delta = 1) { updateScore (PlayerSide::Right, delta); }

  void leftDec (int delta = 1) { updateScore (PlayerSide::Left, -delta); }

  void rightDec (int delta = 1) { updateScore (PlayerSide::Right, -delta); }

  // 从下往上擦去的动画
  ~ScoreBoard ()
  {
    for (int row = FrameHeight; row >= 0; --row) {
      move_cursor (row, 0);
      clear_to_eol ();
      sleep_ms (MSPerTick);
    }
  }
};

#endif	  // SCOREBOARD_HPP
