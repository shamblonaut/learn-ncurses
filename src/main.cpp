#include <ncurses.h>

#include <clocale>
#include <cmath>
#include <iostream>

#include "Text.hpp"
#include "Window.hpp"

int main() {
  // Initialize curses mode
  initscr();
  if (!has_colors()) {
    endwin();
    std::cerr << "Your terminal does not support color\n";
  }
  use_default_colors();
  start_color();

  setlocale(LC_ALL, "");
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(FALSE);
  refresh();

  // Get screen size
  int screenHeight, screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);
  /* mvprintw(0, 0, "Height: %d, Width: %d", screenHeight, screenWidth); */
  /* mvprintw(0, 0, "Maximum Colors: %d", COLORS); */

  // Main section
  int y = 5, x = 5, height = screenHeight / 4, width = screenWidth / 4;

  Window windows[][3] = {
    { Window((Position){ y, x }, (Size){ height, width }, NONE),
      Window((Position){ y, (int)floor((float)screenWidth / 3 + x) },
             (Size){ height, width }, SIMPLE),
      Window((Position){ y, (int)floor((float)screenWidth / 3 * 2 + x) },
             (Size){ height, width }, BLOCK) },
    { Window((Position){ (int)floor((float)screenHeight / 2 + y), x },
             (Size){ height, width }, FLAT),
      Window((Position){ (int)floor((float)screenHeight / 2 + y),
                         (int)floor((float)screenWidth / 3 + x) },
             (Size){ height, width }, DOUBLE),
      Window((Position){ (int)floor((float)screenHeight / 2 + y),
                         (int)floor((float)screenWidth / 3 * 2 + x) },
             (Size){ height, width }, ROUNDED) }
  };

  windows[0][0].addText("No borders", MIDDLE_CENTER);
  windows[0][1].addText("Simple borders", MIDDLE_CENTER);
  windows[0][2].addText("Block borders", MIDDLE_CENTER);
  windows[1][0].addText("Flat borders", MIDDLE_CENTER);
  windows[1][1].addText("Double borders", MIDDLE_CENTER);
  windows[1][2].addText("Rounded borders", MIDDLE_CENTER);

  int winI = 0, winJ = 1;
  Window* activeWindow = &windows[winI][winJ];
  activeWindow->focused = true;
  activeWindow->setBorder(activeWindow->borderMode);
  activeWindow->setTitle("Active Window", LEFT);

  int ch = getch();
  while (ch != 'q') {
    int winY = activeWindow->position.y;
    int winX = activeWindow->position.x;
    int winH = activeWindow->size.height;
    int winW = activeWindow->size.width;
    BorderMode winB = activeWindow->borderMode;

    switch (ch) {
      case 'w':
        if (winY > 0) activeWindow->move((Position){ --winY, winX });
        break;
      case 'a':
        if (winX > 0) activeWindow->move((Position){ winY, --winX });
        break;
      case 's':
        if (winY < screenHeight - 1) {
          activeWindow->move((Position){ ++winY, winX });
        }
        break;
      case 'd':
        if (winX < screenWidth - 1) {
          activeWindow->move((Position){ winY, ++winX });
        }
        break;
      case KEY_UP:
        activeWindow->resize((Size){ --winH, winW });
        break;
      case KEY_LEFT:
        activeWindow->resize((Size){ winH, --winW });
        break;
      case KEY_DOWN:
        activeWindow->resize((Size){ ++winH, winW });
        break;
      case KEY_RIGHT:
        activeWindow->resize((Size){ winH, ++winW });
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
      activeWindow->setTitle("", LEFT);
      activeWindow->render();
      activeWindow->focused = false;
      activeWindow = &windows[winI][winJ];
    }

    activeWindow->setTitle("Active Window", LEFT);
    activeWindow->render();
    activeWindow->focused = true;

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
