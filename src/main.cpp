#include <ncurses.h>

#include <clocale>
#include <cmath>
#include <iostream>

#include "Border.hpp"
#include "Color.hpp"
#include "Text.hpp"
#include "Window.hpp"

struct ActiveWindow {
  Window* window{nullptr};
  std::shared_ptr<Text> marker;
} activeWindow;

void changeActiveWindow(Window* next);

int main() {
  // Initialize curses mode
  initscr();

  // Color stuff
  if (!has_colors()) {
    endwin();
    std::cerr << "Your terminal does not support color\n";
  }
  use_default_colors();
  start_color();

  // Visual stuff
  setlocale(LC_ALL, "");
  cbreak();
  noecho();
  curs_set(FALSE);

  // Input stuff
  keypad(stdscr, TRUE);

  refresh();

  // Get screen size
  int screenHeight, screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);
  /* mvprintw(0, 0, "Height: %d, Width: %d", screenHeight, screenWidth); */
  /* mvprintw(0, 0, "Maximum Colors: %d", COLORS); */

  mvprintw(LINES - 3, 0, "Use WASD to move windows");
  mvprintw(LINES - 2, 0, "Use Arrow Keys to resize windows");
  mvprintw(LINES - 1, 0, "Use HJKL to change focus");

  // Main section
  int y = 5, x = 5, height = screenHeight / 4, width = screenWidth / 4;

  Window windows[][3] = {
      {Window(
           (Position){y, x}, (Size){height, width}, (Title){""}, Border::NONE,
           NEUTRAL
       ),
       Window(
           (Position){y, (int)floor((float)screenWidth / 3 + x)},
           (Size){height, width}, (Title){""}, Border::SIMPLE, NEUTRAL
       ),
       Window(
           (Position){y, (int)floor((float)screenWidth / 3 * 2 + x)},
           (Size){height, width}, (Title){""}, Border::BLOCK, NEUTRAL
       )},
      {Window(
           (Position){(int)floor((float)screenHeight / 2 + y), x},
           (Size){height, width}, (Title){""}, Border::FLAT, NEUTRAL
       ),
       Window(
           (Position
           ){(int)floor((float)screenHeight / 2 + y),
             (int)floor((float)screenWidth / 3 + x)},
           (Size){height, width}, (Title){""}, Border::DOUBLE, NEUTRAL
       ),
       Window(
           (Position
           ){(int)floor((float)screenHeight / 2 + y),
             (int)floor((float)screenWidth / 3 * 2 + x)},
           (Size){height, width}, (Title){""}, Border::ROUNDED, NEUTRAL
       )}
  };

  windows[0][0].addText("No borders", MIDDLE_CENTER);
  windows[0][1].addText("Simple borders", MIDDLE_CENTER);
  windows[0][2].addText("Block borders", MIDDLE_CENTER);
  windows[1][0].addText("Flat borders", MIDDLE_CENTER);
  windows[1][1].addText("Double borders", MIDDLE_CENTER);
  windows[1][2].addText("Rounded borders", MIDDLE_CENTER);

  int winI = 0, winJ = 1;
  changeActiveWindow(&windows[winI][winJ]);

  int ch = getch();
  while (ch != 'q') {
    int winY = activeWindow.window->position.y;
    int winX = activeWindow.window->position.x;
    int winH = activeWindow.window->size.height;
    int winW = activeWindow.window->size.width;

    switch (ch) {
      case 'w':
        if (winY > 0) {
          activeWindow.window->move((Position){--winY, winX});
        }
        break;
      case 'a':
        if (winX > 0) activeWindow.window->move((Position){winY, --winX});
        break;
      case 's':
        if (winY < screenHeight - 1) {
          activeWindow.window->move((Position){++winY, winX});
        }
        break;
      case 'd':
        if (winX < screenWidth - 1) {
          activeWindow.window->move((Position){winY, ++winX});
        }
        break;
      case KEY_UP:
        activeWindow.window->resize((Size){--winH, winW});
        break;
      case KEY_LEFT:
        activeWindow.window->resize((Size){winH, --winW});
        break;
      case KEY_DOWN:
        activeWindow.window->resize((Size){++winH, winW});
        break;
      case KEY_RIGHT:
        activeWindow.window->resize((Size){winH, ++winW});
        break;
      case 'h':
        if (winJ > 0) winJ--;
        break;
      case 'j':
        if (winI < 1) winI++;
        break;
      case 'k':
        if (winI > 0) winI--;
        break;
      case 'l':
        if (winJ < 2) winJ++;
        break;
    }

    if (ch == 'h' || ch == 'j' || ch == 'k' || ch == 'l') {
      changeActiveWindow(&windows[winI][winJ]);
    }

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 3; j++) {
        windows[i][j].render();
      }
    }

    ch = getch();
  }

  // Close curses mode
  endwin();

  return 0;
}

void changeActiveWindow(Window* next) {
  if (activeWindow.window != nullptr) {
    activeWindow.window->winBorder.restore();
    activeWindow.window->removeText(activeWindow.marker);
    activeWindow.window->render();
  }

  activeWindow.window = next;

  activeWindow.window->winBorder.setColor(GREEN, false);
  activeWindow.marker =
      activeWindow.window->addText("*", TOP_LEFT, BLUE, (Offset){0, -1});
  activeWindow.window->render();
}
