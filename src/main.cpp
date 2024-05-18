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

  Position changePressPosition;

  bool moving = false;
  Position preChangePosition;

  bool resizing = false;
  Size preChangeSize;
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
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  mouseinterval(0);
  MEVENT mouseEvent;
  bool mouseUp = true, mouseDown = false;
  printf("\033[?1003h\n");  // Makes the terminal report mouse movement

  refresh();

  // Get screen size
  int screenHeight, screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);
  /* mvprintw(0, 0, "Height: %d, Width: %d", screenHeight, screenWidth); */
  /* mvprintw(0, 0, "Maximum Colors: %d", COLORS); */

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

  Window infoBox = Window(
      (Position){screenHeight - 7, 0}, (Size){7, screenWidth / 4 + 10},
      Title("Instructions", LEFT, BLUE, 0), Border::ROUNDED, CYAN
  );
  infoBox.addText(
      "- Use WASD to move windows", TOP_LEFT, GREEN, (Offset){0, 0}
  );
  infoBox.addText(
      "- Use Arrow Keys to resize windows", TOP_LEFT, GREEN, (Offset){1, 0}
  );
  infoBox.addText(
      "- Use HJKL to change focus", TOP_LEFT, GREEN, (Offset){2, 0}
  );
  infoBox.addText(
      "- Click on a window to focus", TOP_LEFT, GREEN, (Offset){3, 0}
  );
  infoBox.addText(
      "- Click and drag on window to move it", TOP_LEFT, GREEN, (Offset){4, 0}
  );
  infoBox.addText(
      "- Click and drag the right/bottom border to resize window", TOP_LEFT,
      GREEN, (Offset){4, 0}
  );

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

    infoBox.render();

    bool mouseClicked = ch == KEY_MOUSE && getmouse(&mouseEvent) == OK;

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 3; j++) {
        if (mouseClicked) {
          if (mouseEvent.bstate == BUTTON1_PRESSED) {
            mouseUp = false;
            mouseDown = true;
          } else if (mouseEvent.bstate == BUTTON1_RELEASED) {
            mouseUp = true;
            mouseDown = false;
          }

          if (wenclose(windows[i][j].win, mouseEvent.y, mouseEvent.x)) {
            if (!activeWindow.moving &&
                mouseEvent.bstate != REPORT_MOUSE_POSITION) {
              changeActiveWindow(&windows[i][j]);
            }
            if (mouseEvent.bstate == BUTTON1_PRESSED) {
              if (mouseEvent.y == activeWindow.window->position.y +
                                      activeWindow.window->size.height - 1 ||
                  mouseEvent.x == activeWindow.window->position.x +
                                      activeWindow.window->size.width - 1) {
                activeWindow.resizing = true;
                activeWindow.preChangeSize = activeWindow.window->size;
              } else {
                activeWindow.moving = true;
                activeWindow.preChangePosition = activeWindow.window->position;
              }

              activeWindow.changePressPosition =
                  (Position){mouseEvent.y, mouseEvent.x};

              activeWindow.window->winBorder.setColor(MAGENTA, false);
            }
          } else if (activeWindow.moving || activeWindow.resizing) {
            if (mouseUp) {
              activeWindow.moving = false;
              activeWindow.resizing = false;
              activeWindow.window->winBorder.setColor(GREEN, false);
            } else if (mouseDown &&
                       mouseEvent.bstate == REPORT_MOUSE_POSITION) {
              if (activeWindow.moving) {
                activeWindow.window->move((Position
                ){activeWindow.preChangePosition.y +
                      (mouseEvent.y - activeWindow.changePressPosition.y),
                  activeWindow.preChangePosition.x +
                      (mouseEvent.x - activeWindow.changePressPosition.x)});
              } else if (activeWindow.resizing) {
                activeWindow.window->resize((Size
                ){activeWindow.preChangeSize.height +
                      (mouseEvent.y - activeWindow.changePressPosition.y),
                  activeWindow.preChangeSize.width +
                      (mouseEvent.x - activeWindow.changePressPosition.x)});
              }
              activeWindow.window->winBorder.setColor(MAGENTA, false);
            }
          }
        }

        windows[i][j].render();
      }
    }

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
      activeWindow.window->addText("*", TOP_LEFT, BLUE, (Offset){0, -1});
  activeWindow.window->render();
}
