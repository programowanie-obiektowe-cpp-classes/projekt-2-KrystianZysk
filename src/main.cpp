#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int CELL_SIZE = 10;
const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

void initializeGrid(std::vector<std::vector<int>>& grid) {
    srand(static_cast<unsigned>(time(nullptr)));
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            grid[y][x] = rand() % 2;
        }
    }
}

int countNeighbors(const std::vector<std::vector<int>>& grid, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (!(dx == 0 && dy == 0)) {
                int nx = (x + dx + GRID_WIDTH) % GRID_WIDTH;
                int ny = (y + dy + GRID_HEIGHT) % GRID_HEIGHT;
                count += grid[ny][nx];
            }
        }
    }
    return count;
}

void updateGrid(const std::vector<std::vector<int>>& current, std::vector<std::vector<int>>& next) {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            int neighbors = countNeighbors(current, x, y);

            if (current[y][x] == 1) { // jeżeli żyje
                if (neighbors == 2 || neighbors == 3) {
                    next[y][x] = 1; // to żyje dalej
                } else {
                    next[y][x] = 0; // umiera
                }
            } else { // jeżeli martwa
                if (neighbors == 3) {
                    next[y][x] = 1; // ożywa
                } else {
                    next[y][x] = 0; // pozostaje martwa
                }
            }
        }
    }
}

void renderGrid(SDL_Renderer* renderer, const std::vector<std::vector<int>>& grid) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (grid[y][x] == 1) {
                SDL_Rect cell = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = SDL_CreateWindow("Gra w życie", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<std::vector<int>> grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0));
    std::vector<std::vector<int>> nextGrid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0));

    initializeGrid(grid);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        updateGrid(grid, nextGrid);
        grid.swap(nextGrid);

        renderGrid(renderer, grid);

        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
