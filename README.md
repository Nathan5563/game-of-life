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
# Build
make

# Run
./gol
