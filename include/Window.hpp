#pragma once

#include <ncurses.h>

#include <string>
#include <vector>

#include "../include/Text.hpp"

struct Position {
  int y;
  int x;
};

struct Size {
  int height;
  int width;
};

enum BorderMode {
  NONE,
  SIMPLE,
  BLOCK,
  FLAT,
  DOUBLE,
  ROUNDED,
};

class Window {
 public:
  WINDOW* win;

  Position position;
  Size size;

  std::string title;
  std::vector<Text> textContent;

  BorderMode borderMode;
  wchar_t borderGlyphs[8];

  bool focused = false;

  Window(Position position, Size size, BorderMode borderMode);

  void render();
  void setBorder(BorderMode mode);
  void printAligned(const std::string& message, Alignment alignment,
                    Position offset = (Position){ 0, 0 });

  void move(Position newPosition);
  void resize(Size newSize);

  void addText(const std::string& content, Alignment alignment);
  void setTitle(const std::string& title, RowAlignment alignment);

  ~Window();
};
