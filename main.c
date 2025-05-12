#include <stddef.h>
#include <stdbool.h>
#include "SDL2/SDL.h"

typedef uint8_t BYTE;
typedef uint16_t WORD;

#define MAP_WIDTH 128
#define MAP_HEIGHT 64
#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT)
#define GRID_SIZE 10
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640

int map[MAP_SIZE];
int next_map[MAP_SIZE];

void clear_screen()
{
    for (size_t i = 0; i < MAP_SIZE; ++i)
        map[i] = 0;
}

void print_error(const char* err)
{
    fprintf(stderr, "SDL Error: %s\n", err);
}

void initialize_SDL(SDL_Window** window, SDL_Surface** surface)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        print_error(SDL_GetError());
    else
    {
        *window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_RESIZABLE);
        if(!*window) 
            print_error(SDL_GetError());
        else
        {
            *surface = SDL_GetWindowSurface(*window);
            if (!*surface) print_error(SDL_GetError());
        }
    }
}

int get_index(int x, int y)
{
    return (y * MAP_WIDTH) + x;
}

void draw(SDL_Window** window, SDL_Surface** surface, BYTE x, BYTE y)
{
    x = x % MAP_WIDTH;
    y = y % MAP_HEIGHT;

    int idx = get_index(x, y);
    map[idx] = map[idx] ? 0 : 1;

    SDL_Rect rect;
    rect.x = x * GRID_SIZE;
    rect.y = y * GRID_SIZE;
    rect.w = GRID_SIZE;
    rect.h = GRID_SIZE;

    Uint32 color = map[idx] ? SDL_MapRGB((*surface)->format, 255, 255, 255)
                            : SDL_MapRGB((*surface)->format, 0, 0, 0);

    if (SDL_FillRect(*surface, &rect, color) < 0)
        print_error(SDL_GetError());

    SDL_UpdateWindowSurface(*window);
}

void destroy_SDL(SDL_Window** window)
{
    SDL_DestroyWindow(*window);
    SDL_Quit();
}

void update_next_gen()
{
    for (int y = 0; y < MAP_HEIGHT; ++y) 
    {
        for (int x = 0; x < MAP_WIDTH; ++x) 
        {
            int idx = get_index(x, y), live = 0;
            for (int dy = -1; dy <= 1; ++dy) 
            {
                for (int dx = -1; dx <= 1; ++dx) 
                {
                    if (dx == 0 && dy == 0) 
                        continue;
                    int nx = (x + dx + MAP_WIDTH) % MAP_WIDTH;
                    int ny = (y + dy + MAP_HEIGHT) % MAP_HEIGHT;
                    if (map[get_index(nx, ny)]) 
                        ++live;
                }
            }
            next_map[idx] = map[idx] ? (live == 2 || live == 3) : (live == 3);
        }
    }
}

void draw_next_gen(SDL_Window **window, SDL_Surface **surface)
{
    SDL_FillRect(*surface, NULL, SDL_MapRGB((*surface)->format, 0, 0, 0));
    for (int i = 0; i < MAP_SIZE; ++i) 
    {
        if (map[i]) 
        {
            int x = i % MAP_WIDTH;
            int y = i / MAP_WIDTH;
            SDL_Rect r = { 
                x*GRID_SIZE, 
                y*GRID_SIZE,
                GRID_SIZE, 
                GRID_SIZE 
            };
            Uint32 white = SDL_MapRGB((*surface)->format, 255, 255, 255);
            SDL_FillRect(*surface, &r, white);
        }
    }
    SDL_UpdateWindowSurface(*window);
}

int main(int argc, char *argv[])
{
    SDL_Window* window = NULL;
    SDL_Surface* surface = NULL;
    initialize_SDL(&window, &surface);
    clear_screen();

    draw(&window, &surface, 50, 20);
    draw(&window, &surface, 51, 20);
    draw(&window, &surface, 51, 21);
    draw(&window, &surface, 52, 21);
    draw(&window, &surface, 50, 22);

    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT)
                quit = true;
        }

        update_next_gen(next_map);
        memcpy(map, next_map, sizeof(map));
        draw_next_gen(&window, &surface);

        SDL_Delay(250);
    }

    destroy_SDL(&window);
    return 0;
}