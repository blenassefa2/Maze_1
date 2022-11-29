#ifndef HELPER2_H
#define HELPER2_H

#include "../resources/All_Textures.ppm"
#include "../resources/sprites.ppm"
#include "../resources/sky.ppm"
// #include "../resources/title.ppm"
#include "../resources/won.ppm"
#include "../resources/lost.ppm"

float px,py,pdx,pdy,pa;

float px,py,pdx,pdy,pa;
float frame1,frame2,fps;
int gameState=2; //game state. init, start screen, game loop, win/lose
float fade=0;             //the 3 screens can fade up from black

int frames_drawn = 0;
Uint32 timer = 0;
float fps = 0.0f;
Uint32 prev_ticks = SDL_GetTicks;

typedef struct
{
 int w,a,d,s;                     //button state on off
}ButtonKeys; ButtonKeys Keys;

//the map array. Edit to change level but keep the outer walls
int map[]= {
 2,2,2,2,1,2,2,1,
 2,0,0,1,0,0,0,2,
 2,0,0,0,0,4,4,2,
 2,1,4,1,0,0,0,2,
 2,0,0,5,1,4,0,2,
 2,0,0,3,0,1,4,2,
 2,0,0,1,0,0,0,5,
 2,1,2,1,2,1,2,2,	
};
int mapF[]=          //floors
{
 0,0,0,0,0,0,0,0,
 0,0,0,0,2,2,2,0,
 0,0,0,0,6,0,2,0,
 0,0,8,0,2,7,6,0,
 0,0,2,0,0,0,0,0,
 0,0,2,0,8,0,0,0,
 0,1,1,1,1,0,8,0,
 0,0,0,0,0,0,0,0,	
};

int mapC[]=          //ceiling
{
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,4,2,4,0,0,0,0,
 0,0,2,0,0,0,0,0,
 0,0,2,0,0,0,0,0,
 0,0,0,0,0,0,0,0,	
};


typedef struct       //All veriables per sprite
{
 int type;           //static, key, enemy
 int state;          //on off
 int map;            //texture to show
 float x,y,z;        //position
}sprite; sprite sp[4];
int depth[120];      //hold wall line depth to compare for sprite depth


#endif
