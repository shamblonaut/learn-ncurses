#pragma once

#include <string>

#include "Color.hpp"

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

struct Offset {
  Offset() : y(0), x(0) {}
  Offset(int y, int x) : y(y), x(x) {}

  int y;
  int x;
};

struct Text {
  Text(const std::wstring& content)
      : content(content),
        alignment(TOP_LEFT),
        color(NEUTRAL),
        offset(Offset(0, 0)) {}

  Text(
      const std::wstring& content, Alignment alignment, Color color,
      Offset offset
  )
      : content(content), alignment(alignment), color(color), offset(offset) {}

  std::wstring content;
  Alignment alignment;
  Color color;
  Offset offset;
};

struct Title {
  Title(const std::wstring& title)
      : title(title), alignment(LEFT), color(NEUTRAL), offset(0) {}

  Title(
      const std::wstring& title, RowAlignment alignment, Color color, int offset
  )
      : title(title), alignment(alignment), color(color), offset(offset) {}

  std::wstring title;
  RowAlignment alignment{LEFT};
  Color color{NEUTRAL};
  int offset{0};
};
