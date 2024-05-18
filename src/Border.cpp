#include "Border.hpp"

#include "Color.hpp"

Border::Border(BorderMode mode, Color color) : mode(mode), color(color) {
  setColor(color);
  setMode(mode, color);

  initializeColorPairs();
}

void Border::setMode(BorderMode newMode, Color color, bool save) {
  if (save) {
    mode = newMode;
  }

  switch (mode) {
    case NONE:
      setcchar(&characters.left, L" ", 0, color, nullptr);
      setcchar(&characters.right, L" ", 0, color, nullptr);
      setcchar(&characters.top, L" ", 0, color, nullptr);
      setcchar(&characters.bottom, L" ", 0, color, nullptr);
      setcchar(&characters.topLeft, L" ", 0, color, nullptr);
      setcchar(&characters.bottomRight, L" ", 0, color, nullptr);
      setcchar(&characters.topRight, L" ", 0, color, nullptr);
      setcchar(&characters.bottomLeft, L" ", 0, color, nullptr);
      break;
    case SIMPLE:
      setcchar(&characters.left, L"|", 0, color, nullptr);
      setcchar(&characters.right, L"|", 0, color, nullptr);
      setcchar(&characters.top, L"-", 0, color, nullptr);
      setcchar(&characters.bottom, L"-", 0, color, nullptr);
      setcchar(&characters.topLeft, L"+", 0, color, nullptr);
      setcchar(&characters.bottomRight, L"+", 0, color, nullptr);
      setcchar(&characters.topRight, L"+", 0, color, nullptr);
      setcchar(&characters.bottomLeft, L"+", 0, color, nullptr);
      break;
    case BLOCK:
      setcchar(&characters.left, L"█", 0, color, nullptr);
      setcchar(&characters.right, L"█", 0, color, nullptr);
      setcchar(&characters.top, L"▄", 0, color, nullptr);
      setcchar(&characters.bottom, L"▀", 0, color, nullptr);
      setcchar(&characters.topLeft, L"▄", 0, color, nullptr);
      setcchar(&characters.bottomRight, L"▀", 0, color, nullptr);
      setcchar(&characters.topRight, L"▄", 0, color, nullptr);
      setcchar(&characters.bottomLeft, L"▀", 0, color, nullptr);
      break;
    case FLAT:
      setcchar(&characters.left, L"│", 0, color, nullptr);
      setcchar(&characters.right, L"│", 0, color, nullptr);
      setcchar(&characters.top, L"─", 0, color, nullptr);
      setcchar(&characters.bottom, L"─", 0, color, nullptr);
      setcchar(&characters.topLeft, L"┌", 0, color, nullptr);
      setcchar(&characters.bottomRight, L"┘", 0, color, nullptr);
      setcchar(&characters.topRight, L"┐", 0, color, nullptr);
      setcchar(&characters.bottomLeft, L"└", 0, color, nullptr);
      break;
    case DOUBLE:
      setcchar(&characters.left, L"║", 0, color, nullptr);
      setcchar(&characters.right, L"║", 0, color, nullptr);
      setcchar(&characters.top, L"═", 0, color, nullptr);
      setcchar(&characters.bottom, L"═", 0, color, nullptr);
      setcchar(&characters.topLeft, L"╔", 0, color, nullptr);
      setcchar(&characters.bottomRight, L"╝", 0, color, nullptr);
      setcchar(&characters.topRight, L"╗", 0, color, nullptr);
      setcchar(&characters.bottomLeft, L"╚", 0, color, nullptr);
      break;
    case ROUNDED:
      setcchar(&characters.left, L"│", 0, color, nullptr);
      setcchar(&characters.right, L"│", 0, color, nullptr);
      setcchar(&characters.top, L"─", 0, color, nullptr);
      setcchar(&characters.bottom, L"─", 0, color, nullptr);
      setcchar(&characters.topLeft, L"╭", 0, color, nullptr);
      setcchar(&characters.bottomRight, L"╯", 0, color, nullptr);
      setcchar(&characters.topRight, L"╮", 0, color, nullptr);
      setcchar(&characters.bottomLeft, L"╰", 0, color, nullptr);
      break;
  }
}

void Border::setColor(Color newColor, bool save) {
  if (!save) {
    setMode(mode, newColor);
  } else {
    color = newColor;
  }
}

void Border::restore() { setMode(mode, color); }
