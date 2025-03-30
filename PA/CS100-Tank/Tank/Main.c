/// \file
/// \brief Welcome to CS100 Tank!
/// It seems to be a really large project...
/// But don't worry, we do not intend to make it difficult.
/// Enjoy this homework and good luck, OwO!
///
/// Getting started:
/// 1. Compile with `gcc Tank/Main.c`.
/// 2. Run the executable.
/// 3. Move with "WASD" and shoot with "K".
/// 4. Exit with "ESC".

//
//
//
//
//
#include "Game.h"

int main(void) {
  // Uses the current time as a seed for the pseudo-random number generator.
  srand(time(NULL));

  // Configure the game.
  config.fps = 30;
  config.mapSize = (Vec){80, 50};
  config.nEnemies = 8;
  config.nSolids = 8;
  config.nWalls = 5;

  // Run the game.
  GameLifecycle();

  return 0;
}
