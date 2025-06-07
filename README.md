# Game of Life

A C implementation of Conway’s Game of Life using SDL2.

## Features
- Toroidal grid wrapping (128×64 cells) via modulo arithmetic in neighbor calculations
- Fixed grid cell size (10 px)
- Preloaded glider pattern in `main.c`
- Automatic generation update every 250 ms

## Build & Run
Requires SDL2 development libraries.

```sh
# Clone
git clone git@github.com:Nathan5563/game-of-life.git

# Build
make

# Run
./gol
```

## Controls
- The simulation advances automatically.
- Close the window to exit.
- Change initial pattern: edit the `draw(&window, &surface, x, y);` calls near the start of main() in main.c to toggle different cells for custom patterns.
