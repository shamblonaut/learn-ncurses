#include "Window.hpp"

#include <algorithm>

#include "Border.hpp"
#include "Color.hpp"
#include "Text.hpp"

Window::Window(
    Position position, Size size, Title title, Border::BorderMode borderMode,
    Color borderColor
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

  for (std::shared_ptr<Text> text : textContent) {
    printText(*text);
  }

  wrefresh(win);
}

void Window::move(Position newPosition) {
  int screenHeight, screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);

  if (newPosition.y < 0) {
    newPosition.y = 0;
  } else if (newPosition.y + size.height > screenHeight) {
    newPosition.y = screenHeight - size.height;
  }
  if (newPosition.x < 0) {
    newPosition.x = 0;
  } else if (newPosition.x + size.width > screenWidth) {
    newPosition.x = screenWidth - size.width;
  }
  wclear(win);
  wrefresh(win);
  position = newPosition;
  mvwin(win, position.y, position.x);
  render();
}

void Window::resize(Size newSize) {
  int screenHeight, screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);
  if (newSize.height < 0) {
    newSize.height = 0;
  } else if (position.y + newSize.height > screenHeight) {
    newSize.height = screenHeight - position.y;
  }
  if (newSize.width < 0) {
    newSize.width = 0;
  } else if (position.x + newSize.width > screenWidth) {
    newSize.width = screenWidth - position.x;
  }

  wclear(win);
  wrefresh(win);
  size = newSize;
  wresize(win, size.height, size.width);
  render();
}

std::shared_ptr<Text> Window::addText(
    const std::wstring& content, Alignment alignment, Color color, Offset offset
) {
  std::shared_ptr<Text> text =
      std::make_shared<Text>(content, alignment, color, offset);
  textContent.push_back(text);
  render();

  return text;
}

void Window::removeText(std::shared_ptr<Text> text) {
  auto iterator = std::find(textContent.begin(), textContent.end(), text);
  if (iterator != textContent.end()) {
    textContent.erase(iterator);
  }
}

void Window::printText(Text text) {
  int row = text.offset.x, column = text.offset.y;
  switch (text.alignment) {
    case TOP_LEFT:
      row = 1;
      column = 1;
      break;
    case TOP_CENTER:
      row = 1;
      column = size.width / 2 - text.content.length() / 2;
      break;
    case TOP_RIGHT:
      row = 1;
      column = size.width - text.content.length() - 2;
      break;
    case MIDDLE_LEFT:
      row = size.height / 2;
      column = 1;
      break;
    case MIDDLE_CENTER:
      row = size.height / 2;
      column = size.width / 2 - text.content.length() / 2;
      break;
    case MIDDLE_RIGHT:
      row = size.height / 2;
      column = size.width - text.content.length() - 2;
      break;
    case BOTTOM_LEFT:
      row = size.height - 1;
      column = 1;
      break;
    case BOTTOM_CENTER:
      row = size.height - 1;
      column = size.width / 2 - text.content.length() / 2;
      break;
    case BOTTOM_RIGHT:
      row = size.height - 1;
      column = size.width - text.content.length() - 2;
      break;
  }

  wattron(win, COLOR_PAIR(text.color));
  mvwaddwstr(
      win, row + text.offset.y, column + text.offset.x, text.content.c_str()
  );
  wattroff(win, COLOR_PAIR(text.color));
}

void Window::setBorder(Border::BorderMode borderMode, Color color) {
  winBorder.setColor(color);
  winBorder.setMode(borderMode, color);
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
  if (title.title == L"") {
    drawBorder();
    return;
  }

  Alignment printAlignment;
  Offset printOffset;
  switch (title.alignment) {
    case LEFT:
      printAlignment = TOP_LEFT;
      printOffset = (Offset){-1, title.offset + 1};
      break;
    case CENTER:
      printAlignment = TOP_CENTER;
      printOffset = (Offset){-1, title.offset + 0};
      break;
    case RIGHT:
      printAlignment = TOP_RIGHT;
      printOffset = (Offset){-1, title.offset - 1};
      break;
  }

  printText((Text){title.title, printAlignment, title.color, printOffset});
}

Window::~Window() {
  if (!isendwin()) {
    wclear(win);
    wrefresh(win);
  }
  delwin(win);
}
