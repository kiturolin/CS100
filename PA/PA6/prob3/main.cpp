#include "ScoreBoard_hint1.hpp"
#include "CommandController.hpp"

int main() {
  clear_screen ();
  ScoreBoard scoreBoard("Ma Long", "Fan Zhendong", detail::PadLayout::Vertical);

  CommandController cmdCtl{"Enter command:", 2 * FrameHeight + 2};

  while (true) {
    auto cmd = cmdCtl.receiveCommand();
    if (cmd == "q" || cmd == "quit")
      break;
    if (cmd == "l+")
      scoreBoard.leftInc();
    else if (cmd == "l-")
      scoreBoard.leftDec();
    else if (cmd == "r+")
      scoreBoard.rightInc();
    else if (cmd == "r-")
      scoreBoard.rightDec();
    else if (cmd == "lg+")
      scoreBoard.leftGamesInc();
    else if (cmd == "rg+")
      scoreBoard.rightGamesInc();
  }

  return 0;
}
