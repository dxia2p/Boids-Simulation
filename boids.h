#ifndef BOIDS
#define BOIDS

#include <stdlib.h>
#include <stdio.h>

struct Boid;

void initBoids(int size, SDL_Renderer* rend);
void updateAllBoids();
void renderAllBoids();
void cleanBoids();

#endif