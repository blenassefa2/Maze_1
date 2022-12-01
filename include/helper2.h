#ifndef HELPER2_H
#define HELPER2_H

#include "../resources/brick1.ppm"
#include "../resources/brick.ppm"
#include "../resources/door2.ppm"
#include "../resources/torch.ppm"
#include "../resources/grass.ppm"
#include "../resources/sky.ppm"
#include "../resources/AllTextures.ppm"
float px,py,pdx,pdy,pa;
float frame1,frame2,fps;
//the map array. Edit to change level but keep the outer walls

// int All_Textures [3][32*32];

// All_Textures[0] = texture2;

int map[]= {
 1,1,1,1,2,1,1,1,
 1,0,0,1,0,0,0,1,
 1,0,0,3,0,1,0,1,
 1,1,3,1,0,3,0,2,
 1,0,0,0,0,0,0,2,
 1,2,0,1,0,3,0,2,
 1,0,0,3,0,0,0,1,
 1,1,1,1,1,1,1,1,
};

int mapF[]=          //floors
{
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,1,0,0,0,0,0,0,
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

#endif
