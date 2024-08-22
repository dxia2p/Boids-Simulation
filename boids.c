#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "boids.h"

struct Boid{
    int x;
    int y;
};

static struct Boid* boidsArray;   
static int boidsArraySize;
static SDL_Renderer* renderer;
static SDL_Texture* boidTexture;

static SDL_Rect srcR, destR;

void initBoids(int size, SDL_Renderer* rend){
    boidsArray = malloc(size * sizeof(struct Boid));
    boidsArraySize = size;
    renderer = rend;

    destR.w = 30;
    destR.h = 30;

    for(int i = 0; i < size; i++){
        boidsArray[i].x = 0;
        boidsArray[i].y = 0;
    }

    SDL_Surface* tmpBoidsSurface = IMG_Load("assets/Bird.png");
    boidTexture = SDL_CreateTextureFromSurface(renderer, tmpBoidsSurface);
    SDL_FreeSurface(tmpBoidsSurface);
}

void renderAllBoids(){ // This should be called in the render function of the main loop
    for(int i = 0; i < boidsArraySize; i++){
        SDL_RenderCopy(renderer, boidTexture, NULL, &destR);
    }
}

void cleanBoids(){
    free(boidsArray);
}