#pragma once

#include <ncurses.h>

enum Color {
  NEUTRAL,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
};

void initializeColorPairs();
