#pragma once

#include <ncurses.h>
#include <panel.h>

#include <memory>
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
  PANEL* panel;

  Position position;
  Size size;

  Title title;
  std::vector<std::shared_ptr<Text>> textContent;

  Border winBorder;

  Window(
      Position position, Size size, Title title = Title(L""),
      Border::BorderMode borderMode = Border::FLAT, Color borderColor = NEUTRAL
  );

  void render();
  void drawBorder();
  void drawTitle();

  void printText(Text text);

  void move(Position newPosition);
  void resize(Size newSize);

  std::shared_ptr<Text> addText(
      const std::wstring& content, Alignment alignment = TOP_LEFT,
      Color color = NEUTRAL, Offset offset = Offset(0, 0)
  );
  void removeText(std::shared_ptr<Text> text);

  void setBorder(Border::BorderMode borderMode, Color color);
  void setTitle(Title newTitle);

  ~Window();
};
