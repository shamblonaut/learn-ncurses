#include <ncurses.h>

#include <clocale>

int i = 0;

#define refresh() \
  refresh();      \
  mvprintw(0, 0, "Refreshed %d times", ++i);

int main() {
  initscr();
  start_color();
  /*assume_default_colors(5, 6);*/
  /*use_default_colors();*/
  setlocale(LC_ALL, "");
  noecho();
  cbreak();
  curs_set(FALSE);
  refresh();

  WINDOW* win = newwin(25, 100, 10, 10);
  box(win, 0, 0);

  struct BorderCharacters {
    cchar_t left;
    cchar_t right;
    cchar_t top;
    cchar_t bottom;

    cchar_t topLeft;
    cchar_t topRight;
    cchar_t bottomLeft;
    cchar_t bottomRight;
  } characters;

  int color = 4;

  setcchar(&characters.left, L"│", 0, color, nullptr);
  setcchar(&characters.right, L"│", 0, color, nullptr);
  setcchar(&characters.top, L"─", 0, color, nullptr);
  setcchar(&characters.bottom, L"─", 0, color, nullptr);
  setcchar(&characters.topLeft, L"╭", 0, color, nullptr);
  setcchar(&characters.bottomRight, L"╯", 0, color, nullptr);
  setcchar(&characters.topRight, L"╮", 0, color, nullptr);
  setcchar(&characters.bottomLeft, L"╰", 0, color, nullptr);

  wborder_set(
      win, &characters.left, &characters.right, &characters.top,
      &characters.bottom, &characters.topLeft, &characters.topRight,
      &characters.bottomLeft, &characters.bottomRight
  );

  init_pair(3, COLOR_WHITE, COLOR_BLACK);
  init_pair(4, COLOR_BLACK, COLOR_WHITE);
  init_pair(5, 7, 0);
  init_pair(6, 0, 7);

  wattron(win, A_BOLD | COLOR_PAIR(3));
  mvwprintw(win, 1, 1, "Hello World!");
  wattroff(win, A_BOLD | COLOR_PAIR(3));

  wattron(win, A_BOLD | COLOR_PAIR(4));
  mvwprintw(win, 2, 1, "Hello World!");
  wattroff(win, A_BOLD | COLOR_PAIR(4));

  wattron(win, A_BOLD | COLOR_PAIR(5));
  mvwprintw(win, 3, 1, "Hello World!");
  wattroff(win, A_BOLD | COLOR_PAIR(5));

  wattron(win, A_BOLD | COLOR_PAIR(6));
  mvwprintw(win, 4, 1, "Hello World!");
  wattroff(win, A_BOLD | COLOR_PAIR(6));

  wrefresh(win);

  getch();

  endwin();

  return 0;
}
