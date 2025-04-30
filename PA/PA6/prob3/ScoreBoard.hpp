#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "ArtDigits.hpp"
#include "utils.hpp"

#include <cassert>
#include <cstdint>

inline constexpr auto MSPerTick = 50;
inline constexpr int NUM_PADS	= 3;

namespace detail
{
enum class PadLayout { Horizontal, Vertical };

template <std::size_t Height, std::size_t Width>
static void
printBlock (const char (&object)[Height][Width], int baseRow, int baseCol)
{
  for (int row = 0; row != Height; ++row) {
    move_cursor (baseRow + row, baseCol);
    std::cout << object[row];
  }
  std::cout << std::flush;
}

}    // namespace detail

class DigitPad
{
public:
  enum class PadType { Games, Points };

private:
  const TextColor mColor;
  const int mBaseRow;
  const int mBaseCol;
  const int mNo;    // 第几块digitalPad, 从0开始计数
  const PadType mPadType;
  int mCurrentDigit;

public:
  DigitPad (TextColor color, int no, PadType padType, int baseRow, int baseCol) :
      mColor{ color },
      mBaseRow{ baseRow },
      mBaseCol{ baseCol },
      mNo{ no },
      mPadType{ padType },
      mCurrentDigit{ 0 }
  {
    print ();
  }

  int getBaseRow () const { return mBaseRow; };

  int getBaseCol () const { return mBaseCol; };

  int getDigit () const { return mCurrentDigit; };

  // 打印外框与初始数字
  void print () const
  {
    PrinterGuard printerGuard;
    ColorGuard withColor (mPadType == PadType::Points ? mColor : TextColor::Normal);
    // 打印外框
    if (mPadType != PadType::Games) detail::printBlock (Frame, mBaseRow, mBaseCol);
    // 打印初始数字
    detail::printBlock (ArtDigit[mCurrentDigit], DigitBaseRow + mBaseRow, DigitBaseCol + mBaseCol);

    ColorGuard withNormal (TextColor::Normal);
    std::cout << std::flush;
  };

  enum class ScrollDirection { Up, Down };

  void updateAndScroll (int newDigit, ScrollDirection scrollDir)
  {
    if (mCurrentDigit == newDigit) return;
    // 绘图的坐标原点在左上角!
    // 如果dir为Up, 那么scroll后的新数字为bottomDigit
    // 如果dir为Down, 那么scroll后的新数字为topDigit
    PrinterGuard printerGuard;
    ColorGuard withColor (mPadType == PadType::Points ? mColor : TextColor::Normal);
    int baseCol	 = mBaseCol + DigitBaseCol;
    int topDigit = scrollDir == ScrollDirection::Up ? mCurrentDigit : newDigit;
    // 如果topDigit为newDigit, 那么bottomDigit为oldDigit, 反之亦然
    int bottomDigit = mCurrentDigit + newDigit - topDigit;
    // 实现翻页的酷炫动画
    // 最外层的for循环每次打印一帧
    for (int tick = 0; tick != DigitHeight + 1; ++tick) {
      int mid = scrollDir == ScrollDirection::Up ? tick : DigitHeight - tick;
      // 每次从Pad的最上方一行开始打印
      int row = mBaseRow + DigitBaseRow;
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

    mCurrentDigit = newDigit;
    ColorGuard withNormal (TextColor::Normal);
    std::cout << std::flush;
  }
};

class Player
{
public:
  enum class Side { Left, Right };

private:
  std::string mName;
  detail::PadLayout mPadLayout;
  Side mSide;
  int mScore;
  int mGames;
  DigitPad mTensPlace;
  DigitPad mOnesPlace;
  DigitPad mGamesPlace;

public:
  Player (std::string name, detail::PadLayout layout, Side side) :
      mName{ std::move (name) },
      mPadLayout{ layout },
      mSide{ side },
      mScore{ 0 },
      mGames{ 0 },
      mTensPlace{ ColorOfSide (side),
		  0 + static_cast<int> (side),
		  DigitPad::PadType::Points,
		  BaseRowOfSide (layout, side),
		  BaseColOfPad (0 + static_cast<int> (side), layout, side) },
      mOnesPlace{ ColorOfSide (side),
		  1 + static_cast<int> (side),
		  DigitPad::PadType::Points,
		  BaseRowOfSide (layout, side),
		  BaseColOfPad (1 + static_cast<int> (side), layout, side) },
      mGamesPlace{ ColorOfSide (side),
		   side == Side::Left ? 2 : 0,
		   DigitPad::PadType::Games,
		   BaseRowOfSide (layout, side),
		   BaseColOfPad (side == Side::Left ? 2 : 0, layout, side) }
  {
    print ();
  }

  int getScore () const { return mScore; };

  // 打印名字
  void print () const
  {
    PrinterGuard printerGuard;
    ColorGuard withColor (TextColor::Normal);
    if (mPadLayout == detail::PadLayout::Horizontal) {
      move_cursor (FrameHeight, BaseColOfPad (1, mPadLayout, mSide));
      std::cout << mName;
    } else {
      move_cursor (BaseRowOfSide (mPadLayout, mSide) + FrameHeight, 0);
      std::cout << mName;
    }

    std::cout << std::flush;
  }

  void updateScore (int delta)
  {
    if (delta == 0) return;
    if ((mScore + delta) < 0) return;
    DigitPad::ScrollDirection scrollDir = delta > 0 ? DigitPad::ScrollDirection::Up : DigitPad::ScrollDirection::Down;
    mScore += delta;
    mTensPlace.updateAndScroll (mScore / 10, scrollDir);
    mOnesPlace.updateAndScroll (mScore % 10, scrollDir);
  }

  void updateGames (int delta)
  {
    assert (delta >= 0);
    mGames += delta;
    mGamesPlace.updateAndScroll (mGames, DigitPad::ScrollDirection::Up);
  }

private:
  static TextColor ColorOfSide (Side side) { return side == Side::Left ? TextColor::Red : TextColor::Blue; }

  static int BaseColOfPad (const int no, detail::PadLayout layout, Side side)
  {
    // 如果是水平排列的记分板
    auto NamePadWidth = FrameWidth;
    if (layout == detail::PadLayout::Horizontal)
      return side == Side::Left ? no * FrameWidth : (no + NUM_PADS) * FrameWidth + ColonWidth;
    return no * FrameWidth + NamePadWidth;
  };

  static int BaseRowOfSide (detail::PadLayout layout, Side side)
  {
    if (layout == detail::PadLayout::Vertical) return side == Side::Left ? 0 : FrameHeight;
    return 0;
  }
};

class ScoreBoard
{
  Player mLeft;
  Player mRight;
  detail::PadLayout mPadLayout;

public:
  ScoreBoard (std::string leftName, std::string rightName, detail::PadLayout layout) :
      mLeft{ std::move (leftName), layout, Player::Side::Left },
      mRight{ std::move (rightName), layout, Player::Side::Right },
      mPadLayout{ layout }
  {
    PrinterGuard printerGuard;
    if (layout == detail::PadLayout::Horizontal)
      // 中间的分隔符: 冒号
      detail::printBlock (Colon, 0, NUM_PADS * FrameWidth);
  }

  void leftInc (int delta = 1) { mLeft.updateScore (delta); }

  void rightInc (int delta = 1) { mRight.updateScore (delta); }

  void leftDec (int delta = 1) { mLeft.updateScore (-delta); }

  void rightDec (int delta = 1) { mRight.updateScore (-delta); }

  void leftGamesInc (int delta = 1) { mLeft.updateGames (delta); }

  void rightGamesInc (int delta = 1) { mRight.updateGames (delta); }

  ~ScoreBoard ()
  {
    for (int row = mPadLayout == detail::PadLayout::Vertical ? 2 * FrameHeight : FrameHeight; row >= 0; --row) {
      move_cursor (row, 0);
      clear_to_eol ();
      sleep_ms (MSPerTick);
    }
  }
};

#endif	  // SCOREBOARD_HPP
