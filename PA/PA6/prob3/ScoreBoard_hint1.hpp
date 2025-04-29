#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "ArtDigits.hpp"
#include "utils.hpp"

inline constexpr auto MSPerTick = 50;

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
  DigitPad (TextColor color, int no, PadType padType, int baseRow, int baseCol);

  int getBaseRow () const { return mBaseRow; };

  int getBaseCol () const { return mBaseCol; };

  int getDigit () const { return mCurrentDigit; };

  void print () const;

  enum class ScrollDirection { Up, Down };

  void updateAndScroll (int newDigit, ScrollDirection scrollDir);
};

class Player
{
  static constexpr int NUM_PADS = 2;
  std::string mName;
  detail::PadLayout mPadLayout;
  DigitPad mTensPlace;
  DigitPad mOnesPlace;

public:
  enum class Side { Left, Right };

  Player (std::string name, detail::PadLayout layout, Side side) :
      mName{ std::move (name) },
      mPadLayout{ layout },
      mTensPlace{
	ColorOfSide (side), 0, DigitPad::PadType::Points, BaseRowOfSide (layout, side), BaseColOfPad (0, layout, side)
      },
      mOnesPlace{
	ColorOfSide (side), 1, DigitPad::PadType::Points, BaseRowOfSide (layout, side), BaseColOfPad (1, layout, side)
      }
  {
  }

  int getScore () const;

  void print () const;

  void updateScore (int delta);

private:
  static TextColor ColorOfSide (Side side);

  static int BaseColOfPad (const int no, detail::PadLayout layout, Side side)
  {
    // 如果是水平排列的记分板
    if (layout == detail::PadLayout::Horizontal)
      return side == Side::Left ? no * FrameWidth : no * FrameWidth + NUM_PADS;
    return no * FrameWidth;
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

public:
  ScoreBoard (std::string leftName, std::string rightName, detail::PadLayout layout) :
      mLeft{ std::move (leftName), layout, Player::Side::Left },
      mRight{ std::move (rightName), layout, Player::Side::Right }
  {
    // TODO:
  }

  void leftInc (int delta = 1) { mLeft.updateScore (delta); }

  void rightInc (int delta = 1) { mRight.updateScore (delta); }

  void leftDec (int delta = 1) { mLeft.updateScore (-delta); }

  void rightDec (int delta = 1) { mRight.updateScore (-delta); }

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

