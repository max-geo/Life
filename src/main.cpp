#include <SDL3/SDL.h>
#include <iostream>
#include "quadtree.h"


int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "PROGRAM STARTED\n";

    SDL_Window* win = SDL_CreateWindow("Life", 300, 200, SDL_WINDOW_BORDERLESS);
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
    
    int quadtree_size = 64;

    Quadtree q = Quadtree(quadtree_size);
    q.insert(Point{3, 0});

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
        // SDL_RenderRects(ren,);
        SDL_RenderPresent(ren);
    }
    
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
