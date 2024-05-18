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

  WINDOW* win = newwin(25, 100, 10, 10);
  box(win, 0, 0);

  std::wstring greeting = L"Hello 👋";
  mvwaddwstr(win, 1, 1, greeting.c_str());

  wrefresh(win);

  getch();

  endwin();

  return 0;
}
