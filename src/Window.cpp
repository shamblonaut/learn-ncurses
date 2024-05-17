#include "Window.hpp"

#include <ncurses.h>

#include "Border.hpp"
#include "Color.hpp"
#include "Text.hpp"

Window::Window(
    Position position, Size size, Title title,
    Border::BorderMode borderMode = Border::FLAT, Color borderColor = NEUTRAL
)
    : position(position),
      size(size),
      title(title),
      winBorder(Border(borderMode, borderColor)) {
  win = newwin(size.height, size.width, position.y, position.x);
  render();
}

void Window::render() {
  drawBorder();
  drawTitle();

  for (const Text& text : textContent) {
    printAligned(text.content, text.alignment);
  }

  wrefresh(win);
}

void Window::move(Position newPosition) {
  wclear(win);
  wrefresh(win);
  position = newPosition;
  mvwin(win, position.y, position.x);
  render();
}

void Window::resize(Size newSize) {
  wclear(win);
  wrefresh(win);
  size = newSize;
  wresize(win, size.height, size.width);
  render();
}

void Window::addText(const std::string& content, Alignment alignment) {
  textContent.push_back((Text){content, alignment});
  render();
}

void Window::printAligned(
    const std::string& message, Alignment alignment, Position offset
) {
  int row = offset.x, column = offset.y;
  switch (alignment) {
    case TOP_LEFT:
      row = 1;
      column = 1;
      break;
    case TOP_CENTER:
      row = 1;
      column = size.width / 2 - message.length() / 2;
      break;
    case TOP_RIGHT:
      row = 1;
      column = size.width - message.length() - 2;
      break;
    case MIDDLE_LEFT:
      row = size.height / 2;
      column = 1;
      break;
    case MIDDLE_CENTER:
      row = size.height / 2;
      column = size.width / 2 - message.length() / 2;
      break;
    case MIDDLE_RIGHT:
      row = size.height / 2;
      column = size.width - message.length() - 2;
      break;
    case BOTTOM_LEFT:
      row = size.height - 1;
      column = 1;
      break;
    case BOTTOM_CENTER:
      row = size.height - 1;
      column = size.width / 2 - message.length() / 2;
      break;
    case BOTTOM_RIGHT:
      row = size.height - 1;
      column = size.width - message.length() - 2;
      break;
  }

  mvwprintw(win, row + offset.y, column + offset.x, "%s", message.c_str());
}

void Window::setBorder(Border::BorderMode borderMode, Color color) {
  winBorder.setColor(color);
  winBorder.setMode(borderMode);
}
void Window::setTitle(Title newTitle) { title = newTitle; }

void Window::drawBorder() {
  wborder_set(
      win, &winBorder.characters.left, &winBorder.characters.right,
      &winBorder.characters.top, &winBorder.characters.bottom,
      &winBorder.characters.topLeft, &winBorder.characters.topRight,
      &winBorder.characters.bottomLeft, &winBorder.characters.bottomRight
  );
}

void Window::drawTitle() {
  if (title.title == "") {
    drawBorder();
    return;
  }

  Alignment printAlignment;
  Position printOffset;
  switch (title.alignment) {
    case LEFT:
      printAlignment = TOP_LEFT;
      printOffset = (Position){-1, 1};
      break;
    case CENTER:
      printAlignment = TOP_CENTER;
      printOffset = (Position){-1, 0};
      break;
    case RIGHT:
      printAlignment = TOP_RIGHT;
      printOffset = (Position){-1, -1};
      break;
  }

  printAligned(title.title, printAlignment, printOffset);
}

Window::~Window() {
  if (!isendwin()) {
    wclear(win);
    wrefresh(win);
  }
  delwin(win);
}
