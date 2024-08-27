#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "boids.h"
#include "utility.h"

struct Boid{
    float x;
    float y;
    float velX;
    float velY;
    float accelX;
    float accelY;
};

static struct Boid* boidsArray;   
static int boidsArraySize;
static SDL_Renderer* renderer;
static SDL_Texture* boidTexture;

static SDL_Rect srcR, destR;

static int visionRadius = 35; // this is for cohesion and alignment
static int separationRadius = 20;
static int alignmentRadius = 45;

static const int MAX_BOID_VELOCITY = 2;
static float cohesionAccelFactor = 0.03f;
static float separationAccelFactor = 0.05f;
static float alignmentAccelFactor = 0.02f;

void initBoids(int size, SDL_Renderer* rend){
    srand(time(0));

    boidsArray = malloc(size * sizeof(struct Boid));
    boidsArraySize = size;
    renderer = rend;

    // Set the size of the boid texture
    destR.w = 15;
    destR.h = 15;

    // Initialize each boid
    for(int i = 0; i < size; i++){
        boidsArray[i].x = ((float)rand() / RAND_MAX) * 800;
        boidsArray[i].y = ((float)rand() / RAND_MAX) * 600;
        double randomAngle = ((double)rand() / RAND_MAX) * 2 * M_PI;
        boidsArray[i].velX = cos(randomAngle) * MAX_BOID_VELOCITY;
        boidsArray[i].velY = sin(randomAngle) * MAX_BOID_VELOCITY;
    }


    // Load the boid texture
    SDL_Surface* tmpBoidsSurface = IMG_Load("assets/Bird.png");
    boidTexture = SDL_CreateTextureFromSurface(renderer, tmpBoidsSurface);
    SDL_FreeSurface(tmpBoidsSurface);
}

void renderAllBoids(){ // This should be called in the render function of the main loop
    for(int i = 0; i < boidsArraySize; i++){
        destR.x = boidsArray[i].x;
        destR.y = boidsArray[i].y;
        float angle = atan2f(boidsArray[i].velY, boidsArray[i].velX);
        SDL_RenderCopyEx(renderer, boidTexture, NULL, &destR, (angle * 180/M_PI) + 90.0f, NULL, 0);
    }
}

void updateAllBoids(){
    for(int i = 0; i < boidsArraySize; i++){
        boidsArray[i].x += boidsArray[i].velX;
        boidsArray[i].y += boidsArray[i].velY;

        boidsArray[i].velX += boidsArray[i].accelX;
        boidsArray[i].velY += boidsArray[i].accelY;

        float vel = sqrtf(powf(fabsf(boidsArray[i].velX), 2) + powf(fabsf(boidsArray[i].velY), 2));
        //printf("x:%f y:%f\n", powf(fabsf(boidsArray[i].velX), 2), powf(fabsf(boidsArray[i].velY), 2));
        if(vel > MAX_BOID_VELOCITY){ // Clamp the velocity of this boid
            boidsArray[i].velX = (boidsArray[i].velX / vel) * MAX_BOID_VELOCITY;
            boidsArray[i].velY = (boidsArray[i].velY / vel) * MAX_BOID_VELOCITY;
        }


        // Check if the boid is going out of bounds
        if(boidsArray[i].x > 800){ // Remember to change this if the window size changes
            boidsArray[i].x = 0;
        }
        else if(boidsArray[i].x < 0){
            boidsArray[i].x = 800;
        }

        if(boidsArray[i].y > 600){
            boidsArray[i].y = 0;
        }else if(boidsArray[i].y < 0){
            boidsArray[i].y = 600;
        }

        // Cohesion rule and separation rule
        int birdInVisionCount = 0;
        float cohesionSumPosX = 0; // sum of all the positions of birds in the surrounding vision radius of this current bird
        float cohesionSumPosY = 0;
        int birdInSeparationDistCount = 0;
        float separationSumPosX = 0;
        float separationSumPosY = 0;
        int birdInAlignmentCount = 0;
        float alignmentSumPosX = 0;
        float alignmentSumPosY = 0;
        for(int j = 0; j < boidsArraySize; j++){
            if(boidsArray[j].x == boidsArray[i].x && boidsArray[j].y == boidsArray[i].y) continue; // see if the bird we are getting the distance to is the same as the current bird we are looping through
            float diffX = boidsArray[j].x - boidsArray[i].x;
            float diffY = boidsArray[j].y - boidsArray[i].y;
            
            float dist = sqrtf(powf(fabsf(diffX), 2) + powf(fabsf(diffY), 2));
            if(dist <= visionRadius){
                cohesionSumPosX += boidsArray[j].x - boidsArray[i].x;
                cohesionSumPosY += boidsArray[j].y - boidsArray[i].y;
                birdInVisionCount++;
            }
            if(dist <= separationRadius){
                separationSumPosX -= boidsArray[j].x - boidsArray[i].x;
                separationSumPosY -= boidsArray[j].y - boidsArray[i].y;
                birdInSeparationDistCount++;
            }
            if(dist <= alignmentRadius){
                alignmentSumPosX += boidsArray[j].velX;
                alignmentSumPosY += boidsArray[j].velY;
                birdInAlignmentCount++;
            }
        }


        if(birdInVisionCount > 0){
            float targetPosX = cohesionSumPosX / birdInVisionCount;
            float targetPosY = cohesionSumPosY / birdInVisionCount;
            normalize(&targetPosX, &targetPosY);
            boidsArray[i].accelX = targetPosX * cohesionAccelFactor;
            boidsArray[i].accelY = targetPosY * cohesionAccelFactor;
        }

        if(birdInSeparationDistCount > 0){
            float targetPosX = separationSumPosX / birdInSeparationDistCount;
            float targetPosY = separationSumPosX / birdInSeparationDistCount;
            normalize(&targetPosX, &targetPosY);
            boidsArray[i].velX += (targetPosX) * separationAccelFactor;
            boidsArray[i].velY += (targetPosY) * separationAccelFactor;
        }

        if(birdInAlignmentCount > 0){
            float averageVelocityX = alignmentSumPosX / birdInAlignmentCount;
            float averageVelocityY = alignmentSumPosY / birdInAlignmentCount;
            normalize(&averageVelocityX, &averageVelocityY);
            boidsArray[i].velX += averageVelocityX * alignmentAccelFactor;
            boidsArray[i].velY += averageVelocityY * alignmentAccelFactor;
        }
    }
}

void cleanBoids(){
    free(boidsArray);
}