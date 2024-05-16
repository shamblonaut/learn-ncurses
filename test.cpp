#include <ncurses.h>

#include <clocale>

int main() {
  initscr();
  setlocale(LC_ALL, "");
  noecho();
  cbreak();
  curs_set(FALSE);
  refresh();

  WINDOW* win = newwin(10, 50, 10, 10);
  wrefresh(win);

  getch();

  endwin();

  return 0;
}
