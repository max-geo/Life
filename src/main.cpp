#include <iostream>
#include "quadtree.h"


void paintRects(SDL_Renderer* ren, std::vector<Cell>& cells, int cell_size, int padding) {
    SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    for (auto cell : cells) {
        SDL_FRect rect {cell.x*cell_size + padding, cell.y*cell_size + padding, cell.length + cell_size - padding, cell.length + cell_size - padding}; 
        SDL_RenderFillRect(ren, &rect);
    }
}


int main(int argc, char *argv[]){
    
    int q_length = 8;
    int cell_size = 10;
    int padding = cell_size / 5;

    Quadtree q = Quadtree(q_length);

    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "PROGRAM STARTED\n";

    SDL_Window* win = SDL_CreateWindow("Life", q_length*cell_size + padding, q_length*cell_size + padding, SDL_WINDOW_BORDERLESS);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl; 
        SDL_Quit();
        return 1;   
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);
    if (ren == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl; 
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;   
    }

    q.insert(Point{3, 0});
    q.insert(Point{4, 0});
    q.insert(Point{3, 1});
    q.insert(Point{0, 0});
    q.insert(Point{6, 6});
    q.insert(Point{7, 7});

    SDL_Event e;

    bool quit = false;

    while (!quit) { // main loop
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(ren);
        std::vector<Cell> cells = q.aliveCells();
        paintRects(ren, cells, cell_size, padding);
        SDL_RenderPresent(ren);
    }
    
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}