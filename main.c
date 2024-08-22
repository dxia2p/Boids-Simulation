#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "boids.h"

SDL_Window* window;
static SDL_Renderer* renderer;

SDL_Rect srcR, destR;

int isRunning = 0;

int cnt = 0; // This is temporary!!! I am using it to see if the game loop works.

int initSDL(){
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
        if(!window){
            printf("Error in creating window\n");
            return 1;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if(!renderer){
            printf("Error in creating renderer\n");
            return 1;
        }

    }else{
        printf("Error in initializing SDL!\n");
        return 1;
    }

    isRunning = 1;
    return 0;

}

void handleEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT:
            isRunning = 0;
            break;
        
        default:
            break;
    }
}

void update(){
    cnt++;
    destR.h = 32;
    destR.w = 32;
    destR.x = cnt;
    printf("%d\n", cnt);
}

void render(){
    SDL_RenderClear(renderer);
    
    // Draw a boid to screen temporarily
    renderAllBoids();

    SDL_RenderPresent(renderer);
}

void clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    printf("Exiting application\n");
}

int main(int argc, char* argv[]){

    int initSuccessful = initSDL();

    if(initSuccessful != 0){
        return 1;
    }

    initBoids(30, renderer);

    // Limit FPS
    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;
    
    while(isRunning){

        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }

    cleanBoids();
    clean();

    return 0;
}