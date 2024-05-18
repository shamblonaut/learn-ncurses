#pragma once

#include <ncurses.h>

#include "Color.hpp"

class Border {
 public:
  enum BorderMode {
    NONE,
    SIMPLE,
    BLOCK,
    FLAT,
    DOUBLE,
    ROUNDED,
  };

  struct BorderCharacters {
    cchar_t left;
    cchar_t right;
    cchar_t top;
    cchar_t bottom;

    cchar_t topLeft;
    cchar_t topRight;
    cchar_t bottomLeft;
    cchar_t bottomRight;
  } characters;

  BorderMode mode;
  Color color;

  Border(BorderMode mode, Color color = NEUTRAL);
  void setMode(BorderMode newMode, Color color, bool save = true);
  void setColor(Color newColor, bool save = true);
  void restore();
};
