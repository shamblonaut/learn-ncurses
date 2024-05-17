#pragma once

#include <ncurses.h>

#include <string>
#include <vector>

#include "Border.hpp"
#include "Color.hpp"
#include "Text.hpp"

struct Position {
  int y;
  int x;
};

struct Size {
  int height;
  int width;
};

class Window {
 public:
  WINDOW* win;

  Position position;
  Size size;

  Title title;
  std::vector<Text> textContent;

  Border winBorder;
  wchar_t borderGlyphs[8];

  bool focused = false;

  Window(
      Position position, Size size, Title title, Border::BorderMode borderMode,
      Color borderColor
  );

  void render();
  void drawBorder();
  void drawTitle();

  void printAligned(
      const std::string& message, Alignment alignment,
      Position offset = (Position){0, 0}
  );

  void move(Position newPosition);
  void resize(Size newSize);

  void addText(const std::string& content, Alignment alignment);
  void setBorder(Border::BorderMode borderMode, Color color);
  void setTitle(Title newTitle);

  ~Window();
};
