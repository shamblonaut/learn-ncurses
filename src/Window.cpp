#include "Window.hpp"

#include <ncurses.h>

#include "Text.hpp"

Window::Window(Position position, Size size, BorderMode borderMode)
    : position(position), size(size), borderMode(borderMode) {
  win = newwin(size.height, size.width, position.y, position.x);
  render();
  init_pair(1, COLOR_GREEN, -1);
}

void Window::render() {
  setBorder(borderMode);
  setTitle(title, LEFT);

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
  textContent.push_back((Text){ content, alignment });
  render();
}

void Window::printAligned(const std::string& message, Alignment alignment,
                          Position offset) {
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
  wrefresh(win);
}

void Window::setTitle(const std::string& newTitle, RowAlignment alignment) {
  title = newTitle;
  if (title == "") {
    setBorder(borderMode);
    return;
  }

  Alignment printAlignment;
  Position printOffset;
  switch (alignment) {
    case LEFT:
      printAlignment = TOP_LEFT;
      printOffset = (Position){ -1, 1 };
      break;
    case CENTER:
      printAlignment = TOP_CENTER;
      printOffset = (Position){ -1, 0 };
      break;
    case RIGHT:
      printAlignment = TOP_RIGHT;
      printOffset = (Position){ -1, -1 };
      break;
  }

  printAligned(title, printAlignment, printOffset);
}

void Window::setBorder(BorderMode mode) {
  if (focused) {
    wattron(win, COLOR_PAIR(1));
  } else {
    wattron(win, COLOR_PAIR(0));
  }

  cchar_t lb, rb, tb, bb, tl, tr, bl, br;
  switch (mode) {
    case NONE:
      wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
      break;
    case SIMPLE:
      wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
      break;
    case BLOCK:
      setcchar(&lb, L"█", 0, focused ? 1 : 0, nullptr);
      setcchar(&rb, L"█", 0, focused ? 1 : 0, nullptr);
      setcchar(&tb, L"▄", 0, focused ? 1 : 0, nullptr);
      setcchar(&bb, L"▀", 0, focused ? 1 : 0, nullptr);
      setcchar(&tl, L"▄", 0, focused ? 1 : 0, nullptr);
      setcchar(&br, L"▀", 0, focused ? 1 : 0, nullptr);
      setcchar(&tr, L"▄", 0, focused ? 1 : 0, nullptr);
      setcchar(&bl, L"▀", 0, focused ? 1 : 0, nullptr);
      break;
    case FLAT:
      setcchar(&lb, L"│", 0, focused ? 1 : 0, nullptr);
      setcchar(&rb, L"│", 0, focused ? 1 : 0, nullptr);
      setcchar(&tb, L"─", 0, focused ? 1 : 0, nullptr);
      setcchar(&bb, L"─", 0, focused ? 1 : 0, nullptr);
      setcchar(&tl, L"┌", 0, focused ? 1 : 0, nullptr);
      setcchar(&br, L"┘", 0, focused ? 1 : 0, nullptr);
      setcchar(&tr, L"┐", 0, focused ? 1 : 0, nullptr);
      setcchar(&bl, L"└", 0, focused ? 1 : 0, nullptr);
      break;
    case DOUBLE:
      setcchar(&lb, L"║", 0, focused ? 1 : 0, nullptr);
      setcchar(&rb, L"║", 0, focused ? 1 : 0, nullptr);
      setcchar(&tb, L"═", 0, focused ? 1 : 0, nullptr);
      setcchar(&bb, L"═", 0, focused ? 1 : 0, nullptr);
      setcchar(&tl, L"╔", 0, focused ? 1 : 0, nullptr);
      setcchar(&br, L"╝", 0, focused ? 1 : 0, nullptr);
      setcchar(&tr, L"╗", 0, focused ? 1 : 0, nullptr);
      setcchar(&bl, L"╚", 0, focused ? 1 : 0, nullptr);
      break;
    case ROUNDED:
      setcchar(&lb, L"│", 0, focused ? 1 : 0, nullptr);
      setcchar(&rb, L"│", 0, focused ? 1 : 0, nullptr);
      setcchar(&tb, L"─", 0, focused ? 1 : 0, nullptr);
      setcchar(&bb, L"─", 0, focused ? 1 : 0, nullptr);
      setcchar(&tl, L"╭", 0, focused ? 1 : 0, nullptr);
      setcchar(&br, L"╯", 0, focused ? 1 : 0, nullptr);
      setcchar(&tr, L"╮", 0, focused ? 1 : 0, nullptr);
      setcchar(&bl, L"╰", 0, focused ? 1 : 0, nullptr);
      break;
  }

  if (mode != NONE && mode != SIMPLE) {
    wborder_set(win, &lb, &rb, &tb, &bb, &tl, &tr, &bl, &br);
  }

  if (focused ? 1 : 0) {
    wattroff(win, COLOR_PAIR(1));
  } else {
    wattroff(win, COLOR_PAIR(0));
  }

  wrefresh(win);
}

Window::~Window() {
  setBorder(NONE);
  wclear(win);
  wrefresh(win);
  delwin(win);
}
