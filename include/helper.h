#ifndef HELPER_H
#define HELPER_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (960)
#define WINDOW_HEIGHT (640)

#define SPEED (300)

//-----------------------------MAP----------------------------------------------
#define mapX  8      //map width
#define mapY  8      //map height
#define mapS 64      //map cube size
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.01754533 //one degreee in radians


void drawMap2D();
void drawPlayer2D();
int buttons(SDL_Event event);
void drawRays2D();
void display();
void init();
void setUp(SDL_Renderer *gameRenderer);
void startGame(SDL_Event event);
float degToRad(int a);
float FixAng(float a);

#endif