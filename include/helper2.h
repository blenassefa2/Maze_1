#ifndef HELPER2_H
#define HELPER2_H

#include "../resources/brick1.ppm"
#include "../resources/brick.ppm"
#include "../resources/door2.ppm"
#include "../resources/finish.ppm"
#include "../resources/grass.ppm"
#include "../resources/sky.ppm"
#include "../resources/title.ppm"
#include "../resources/lost.ppm"
#include "../resources/won.ppm"
#include "../resources/key.ppm"
#include "../resources/diamond.ppm"
#include "../resources/sprites.ppm"
#include "../resources/knight.ppm"

float px,py,pdx,pdy,pa;
float frame1,frame2,fps;
int drawMap = 1;
int gameState = 0, timer = 0;
int keyCount, heartCount, diamondCount;
float fade;
int closeGame = 0;
//the map array. Edit to change level but keep the outer walls

SDL_Texture* keys;
SDL_Texture* stats;
SDL_Texture **scores = NULL;
// int All_Textures [3][32*32];

// All_Textures[0] = texture2;

int map[]= {
 1,1,1,1,2,1,1,1,
 1,0,0,1,0,0,0,1,
 1,0,0,3,0,1,0,1,
 1,1,3,1,0,3,0,2,
 1,0,0,0,0,0,0,2,
 1,2,0,1,0,3,0,2,
 1,0,0,3,0,0,0,4,
 1,1,1,1,1,1,1,1,
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
