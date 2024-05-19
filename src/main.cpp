#include <ncurses.h>
#include <panel.h>

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

  Position changePressPosition;

  bool moving = false;
  Position preChangePosition;

  bool resizing = false;
  Size preChangeSize;
} activeWindow;

void changeActiveWindow(Window* next);
void resetWindowStates(Window windows[2][3]);
void handleMouseEvents(Window* window, MEVENT* mouseEvent);

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
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  mouseinterval(0);
  MEVENT mouseEvent;
  bool mouseUp = true, mouseDown = false;
  printf("\033[?1003h\n");  // Makes the terminal report mouse movement

  // Get screen size
  int screenHeight, screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);
  /* mvprintw(0, 0, "Height: %d, Width: %d", screenHeight, screenWidth); */
  /* mvprintw(0, 0, "Maximum Colors: %d", COLORS); */

  // Main section
  int y = 5, x = 5, height = screenHeight / 4, width = screenWidth / 4;

  Window windows[][3] = {
      {Window(
           (Position){y, x}, (Size){height, width}, (Title){L""}, Border::NONE,
           NEUTRAL
       ),
       Window(
           (Position){y, (int)floor((float)screenWidth / 3 + x)},
           (Size){height, width}, (Title){L""}, Border::SIMPLE, NEUTRAL
       ),
       Window(
           (Position){y, (int)floor((float)screenWidth / 3 * 2 + x)},
           (Size){height, width}, (Title){L""}, Border::BLOCK, NEUTRAL
       )},
      {Window(
           (Position){(int)floor((float)screenHeight / 2 + y), x},
           (Size){height, width}, (Title){L""}, Border::FLAT, NEUTRAL
       ),
       Window(
           (Position
           ){(int)floor((float)screenHeight / 2 + y),
             (int)floor((float)screenWidth / 3 + x)},
           (Size){height, width}, (Title){L""}, Border::DOUBLE, NEUTRAL
       ),
       Window(
           (Position
           ){(int)floor((float)screenHeight / 2 + y),
             (int)floor((float)screenWidth / 3 * 2 + x)},
           (Size){height, width}, (Title){L""}, Border::ROUNDED, NEUTRAL
       )}
  };

  windows[0][0].addText(L"No borders", MIDDLE_CENTER);
  windows[0][1].addText(L"Simple borders", MIDDLE_CENTER);
  windows[0][2].addText(L"Block borders", MIDDLE_CENTER);
  windows[1][0].addText(L"Flat borders", MIDDLE_CENTER);
  windows[1][1].addText(L"Double borders", MIDDLE_CENTER);
  windows[1][2].addText(L"Rounded borders", MIDDLE_CENTER);

  Window infoBox = Window(
      (Position){screenHeight - 8, 0}, (Size){8, screenWidth / 4 + 10},
      Title(L"Instructions", LEFT, BLUE, 0), Border::ROUNDED, CYAN
  );
  infoBox.addText(
      L"• Use WASD to move windows", TOP_LEFT, GREEN, (Offset){0, 0}
  );
  infoBox.addText(
      L"• Use Arrow Keys to resize windows", TOP_LEFT, GREEN, (Offset){1, 0}
  );
  infoBox.addText(
      L"• Use HJKL to change focus", TOP_LEFT, GREEN, (Offset){2, 0}
  );
  infoBox.addText(
      L"• Click on a window to focus", TOP_LEFT, GREEN, (Offset){3, 0}
  );
  infoBox.addText(
      L"• Click and drag on window to move it", TOP_LEFT, GREEN, (Offset){4, 0}
  );
  infoBox.addText(
      L"• Click and drag the right/bottom border to resize window", TOP_LEFT,
      GREEN, (Offset){4, 0}
  );
  infoBox.addText(
      L"• Press r to reset windows to initial state", TOP_LEFT, GREEN,
      (Offset){5, 0}
  );

  int winI = 0, winJ = 1;
  changeActiveWindow(&windows[winI][winJ]);

  doupdate();

  int ch = getch();
  while (ch != 'q') {
    int winY = activeWindow.window->position.y;
    int winX = activeWindow.window->position.x;
    int winH = activeWindow.window->size.height;
    int winW = activeWindow.window->size.width;

    switch (ch) {
      case 'w':
        activeWindow.window->move((Position){--winY, winX});
        break;
      case 'a':
        activeWindow.window->move((Position){winY, --winX});
        break;
      case 's':
        activeWindow.window->move((Position){++winY, winX});
        break;
      case 'd':
        activeWindow.window->move((Position){winY, ++winX});
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
      case 'r':
        resetWindowStates(windows);
        ch = 0;
        continue;
        break;
    }

    if (ch == 'h' || ch == 'j' || ch == 'k' || ch == 'l') {
      changeActiveWindow(&windows[winI][winJ]);
    }

    if (ch == KEY_RESIZE) {
      resetWindowStates(windows);
    }

    infoBox.render();

    bool mouseClicked = ch == KEY_MOUSE && getmouse(&mouseEvent) == OK;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 3; j++) {
        if (mouseClicked) {
          handleMouseEvents(&windows[i][j], &mouseEvent);
        }
        windows[i][j].render();
      }
    }
    doupdate();

    ch = getch();
  }

  printf("\033[?1003h\n");  // Disable mouse movement report

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
      activeWindow.window->addText(L"◆", TOP_LEFT, BLUE, (Offset){0, -1});
  activeWindow.window->render();
}

void resetWindowStates(Window windows[2][3]) {
  int screenHeight, screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);

  int y = 5, x = 5, height = screenHeight / 4, width = screenWidth / 4;

  windows[0][0].move((Position){y, x});
  windows[0][0].resize((Size){height, width});

  windows[0][1].move((Position){y, (int)floor((float)screenWidth / 3 + x)});
  windows[0][1].resize((Size){height, width});

  windows[0][2].move((Position){y, (int)floor((float)screenWidth / 3 * 2 + x)});
  windows[0][2].resize((Size){height, width});

  windows[1][0].move((Position
  ){(int)floor((float)screenHeight / 2 + y), (int)floor((float)x)});
  windows[1][0].resize((Size){height, width});

  windows[1][1].move((Position
  ){(int)floor((float)screenHeight / 2 + y),
    (int)floor((float)screenWidth / 3 + x)});
  windows[1][1].resize((Size){height, width});

  windows[1][2].move((Position
  ){(int)floor((float)screenHeight / 2 + y),
    (int)floor((float)screenWidth / 3 * 2 + x)});
  windows[1][2].resize((Size){height, width});

  windows[0][0].render();
  windows[0][1].render();
  windows[0][2].render();
  windows[1][0].render();
  windows[1][1].render();
  windows[1][2].render();
}

int mouseUp = true, mouseDown = false;
void handleMouseEvents(Window* window, MEVENT* mouseEvent) {
  if (mouseEvent->bstate == BUTTON1_PRESSED) {
    mouseUp = false;
    mouseDown = true;
  } else if (mouseEvent->bstate == BUTTON1_RELEASED) {
    mouseUp = true;
    mouseDown = false;
  }

  if (wenclose(window->win, mouseEvent->y, mouseEvent->x)) {
    if (!activeWindow.moving && mouseEvent->bstate != REPORT_MOUSE_POSITION) {
      changeActiveWindow(window);
    }
    if (mouseEvent->bstate == BUTTON1_PRESSED) {
      if (mouseEvent->y == activeWindow.window->position.y +
                               activeWindow.window->size.height - 1 ||
          mouseEvent->x == activeWindow.window->position.x +
                               activeWindow.window->size.width - 1) {
        activeWindow.resizing = true;
        activeWindow.preChangeSize = activeWindow.window->size;
      } else {
        activeWindow.moving = true;
        activeWindow.preChangePosition = activeWindow.window->position;
      }

      activeWindow.changePressPosition =
          (Position){mouseEvent->y, mouseEvent->x};

      activeWindow.window->winBorder.setColor(MAGENTA, false);
    }
  } else if (activeWindow.moving || activeWindow.resizing) {
    if (mouseUp) {
      activeWindow.moving = false;
      activeWindow.resizing = false;
      activeWindow.window->winBorder.setColor(GREEN, false);
    } else if (mouseDown && mouseEvent->bstate == REPORT_MOUSE_POSITION) {
      if (activeWindow.moving) {
        activeWindow.window->move((Position
        ){activeWindow.preChangePosition.y +
              (mouseEvent->y - activeWindow.changePressPosition.y),
          activeWindow.preChangePosition.x +
              (mouseEvent->x - activeWindow.changePressPosition.x)});
      } else if (activeWindow.resizing) {
        activeWindow.window->resize((Size
        ){activeWindow.preChangeSize.height +
              (mouseEvent->y - activeWindow.changePressPosition.y),
          activeWindow.preChangeSize.width +
              (mouseEvent->x - activeWindow.changePressPosition.x)});
      }
      activeWindow.window->winBorder.setColor(MAGENTA, false);
    }
  }
}
