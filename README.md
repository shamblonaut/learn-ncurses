# learn-ncurses

I try to learn ncurses and its various functionalities

## Run on your local machine

- `cmake` required

```bash
git clone https://github.com/shamblonaut/learn-ncurses
cd learn-ncurses
mkdir build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
make
./tui
```

## Features

- Windows
- Moving Windows
- Resizing Windows
- Changing focus
- Coloring
- Using mouse to move and resize
- Reset windows to proportional size and shape on terminal resize
- Use panels to move and resize windows