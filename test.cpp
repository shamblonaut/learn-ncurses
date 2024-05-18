#include <ncurses.h>

#include <clocale>
#include <string>

int i = 0;

#define refresh() \
  refresh();      \
  mvprintw(0, 0, "Refreshed %d times", ++i);

int main() {
  initscr();
  start_color();
  /*assume_default_colors(5, 6);*/
  use_default_colors();
  setlocale(LC_ALL, "");
  noecho();
  cbreak();
  curs_set(FALSE);
  refresh();

  int screenHeight, screenWidth;
  getmaxyx(stdscr, screenHeight, screenWidth);

  WINDOW* win = newwin(
      screenHeight / 8, screenWidth / 8, screenHeight / 2 - screenHeight / 16,
      screenWidth / 2 - screenWidth / 16
  );
  box(win, 0, 0);

  std::wstring greeting = L"Hello 👋";
  mvwaddwstr(win, 1, 1, greeting.c_str());

  wrefresh(win);

  int ch;
  int i = 0;
  while ((ch = getch()) != 'q') {
    if (ch == KEY_RESIZE) {
      mvaddstr(i++, 0, "Window was resized");
      getmaxyx(stdscr, screenHeight, screenWidth);

      wclear(win);
      wrefresh(win);
      delwin(win);

      win = newwin(
          screenHeight / 4, screenWidth / 4,
          screenHeight / 2 - screenHeight / 8, screenWidth / 2 - screenWidth / 8
      );
      box(win, 0, 0);
      mvwaddwstr(win, 1, 1, greeting.c_str());

      wrefresh(win);
      refresh();
    }
  }

  endwin();

  return 0;
}
