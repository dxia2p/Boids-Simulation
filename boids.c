#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <ctime>

#include "boids.h"

struct Boid{
    float x;
    float y;
    float velX;
    float velY;
};

static struct Boid* boidsArray;   
static int boidsArraySize;
static SDL_Renderer* renderer;
static SDL_Texture* boidTexture;

static SDL_Rect srcR, destR;

static int visionRadius = 50;

static const int BOID_VELOCITY = 20;

void initBoids(int size, int perceptionRadius, int maxSpeed, int separationDistance, SDL_Renderer* rend){
    srand(time(0));

    boidsArray = malloc(size * sizeof(struct Boid));
    boidsArraySize = size;
    renderer = rend;

    destR.w = 30;
    destR.h = 30;

    for(int i = 0; i < size; i++){
        boidsArray[i].x = 0;
        boidsArray[i].y = 0;
        double randomAngle = ((double)rand() / RAND_MAX) * 2 * M_PI;
        boidsArray[i].velX = cos(randomAngle) * BOID_VELOCITY;
        boidsArray[i].velY = sin(randomAngle) * BOID_VELOCITY;
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