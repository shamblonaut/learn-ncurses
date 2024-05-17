#pragma once

#include <string>

enum Alignment {
  TOP_LEFT,
  TOP_CENTER,
  TOP_RIGHT,

  MIDDLE_LEFT,
  MIDDLE_CENTER,
  MIDDLE_RIGHT,

  BOTTOM_LEFT,
  BOTTOM_CENTER,
  BOTTOM_RIGHT
};

enum RowAlignment { LEFT, CENTER, RIGHT };

struct Text {
  std::string content;
  Alignment alignment;
};

struct Title {
  std::string title;
  RowAlignment alignment;
};
