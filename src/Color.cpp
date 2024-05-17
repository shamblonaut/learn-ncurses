#include "Color.hpp"

void initializeColorPairs() {
  short fg, bg;
  pair_content(0, &fg, &bg);

  init_pair(1, COLOR_RED, bg);
  init_pair(2, COLOR_GREEN, bg);
  init_pair(3, COLOR_YELLOW, bg);
  init_pair(4, COLOR_BLUE, bg);
  init_pair(5, COLOR_MAGENTA, bg);
  init_pair(6, COLOR_CYAN, bg);
  init_pair(7, COLOR_WHITE, bg);
}
