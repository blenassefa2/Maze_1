
#include "helper.h"
#include "helper2.h"

void drawMap2D(SDL_Renderer *renderer)
{
 int x, y, xo, yo;
 SDL_Rect tile = {0,0,mapS,mapS};

 for(y=0;y<mapY;y++)
 {
  for(x=0;x<mapX;x++)
  {
    if(map[y*mapX+x]>=1)
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
    else
        SDL_SetRenderDrawColor(renderer,0,0,0,0); 

    xo=x*(mapS + 1); yo=y*(mapS+1);
    tile.x = xo , tile.y = yo;
    SDL_RenderDrawRect(renderer,&tile);
    SDL_RenderFillRect(renderer,&tile);

  } 
 }
}
void drawPixel(SDL_Renderer *renderer,int width, int height, int r, int g, int b, int x, int y)
{
    SDL_Rect pixel = {x, y, width, height};
    
    
    SDL_SetRenderDrawColor(renderer,r,g,b,0);
    SDL_RenderDrawRect(renderer, &pixel);
    SDL_RenderFillRect(renderer,&pixel);
}
void drawPlayer2D(SDL_Renderer *renderer)
{
//  SDL_Rect player = {px, py, 8, 8};
  

 
//  SDL_SetRenderDrawColor(renderer,255,255,0,255);
//  SDL_RenderDrawRect(renderer, &player);
//  SDL_RenderFillRect(renderer,&player);
 drawPixel(renderer, 8,8,255,255,0,px,py);
 
 SDL_RenderDrawLine(renderer, px,py,px+pdx*5,py+pdy*5);
}

int isdoor() 
{
  int xo=0; if(pdx<0){ xo=-25;} else{ xo=25;}
  int yo=0; if(pdy<0){ yo=-25;} else{ yo=25;} 
  int ipx=px/64.0, ipx_add_xo=(px+xo)/64.0;
  int ipy=py/64.0, ipy_add_yo=(py+yo)/64.0;
  if(map[ipy_add_yo*mapX+ipx_add_xo]==3){ return ipy_add_yo*mapX+ipx_add_xo;}
  return -1;
}

int buttons(SDL_Event event)
{
    int door =-1;
    int xo=0; if(pdx<0){ xo=-20;} else{ xo=20;}                                    //x offset to check map
    int yo=0; if(pdy<0){ yo=-20;} else{ yo=20;}                                    //y offset to check map
    int ipx=px/64.0, ipx_add_xo=(px+xo)/64.0, ipx_sub_xo=(px-xo)/64.0;             //x position and offset
    int ipy=py/64.0, ipy_add_yo=(py+yo)/64.0, ipy_sub_yo=(py-yo)/64.0; 
    switch (event.type)
    {
        case SDL_QUIT:
            return -1;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    if(map[ipy*mapX        + ipx_add_xo]==0){ px+=pdx*0.2*fps;}
                    if(map[ipy_add_yo*mapX + ipx       ]==0){ py+=pdy*0.2*fps;}
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    pa+=0.2*fps; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if(map[ipy*mapX        + ipx_sub_xo]==0){ px-=pdy*0.2*fps;}
                    if(map[ipy_sub_yo*mapX + ipx       ]==0){ py-=pdy*0.2*fps;}
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    pa-=0.2*fps; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));
                    break;
                case SDL_SCANCODE_E:
                    door  = isdoor();
                    if (door > -1){map[door] = 0;}
                default:
                    break;
            }
        default:
            break;
    }
    return 0;

}//-----------------------------------------------------------------------------
int all_textures(int hmt, int pixel)
{
    switch (hmt)
    {
        case 0:
            return texture1[pixel];
            break;
        case 1:
            return texture2[pixel];
            break;
        case 2:
            if (door[pixel] == 0)
                return texture1[pixel];
            return door[pixel];
        case 3:
            if (torch[pixel] == 0)
                return texture1[pixel];
            return torch[pixel];
        default:
        break;
    }
    return 0;
}
void drawRays2D(SDL_Renderer *renderer)
{
    int r,mx,my,mp,dof,side; float vx,vy,rx,ry,ra,xo,yo,disV,disH; 
 
 ra=FixAng(pa+30);                                                              //ray set back 30 degrees
 
 for(r=0;r<120;r++)
 {
  int vmt=0,hmt=0;                                                              //vertical and horizontal map texture number 
  //---Vertical--- 
  dof=0; side=0; disV=100000;
  float Tan=tan(degToRad(ra));
       if(cos(degToRad(ra))> 0.001){ rx=(((int)px>>6)<<6)+64;      ry=(px-rx)*Tan+py; xo= 64; yo=-xo*Tan;}//looking left
  else if(cos(degToRad(ra))<-0.001){ rx=(((int)px>>6)<<6) -0.0001; ry=(px-rx)*Tan+py; xo=-64; yo=-xo*Tan;}//looking right
  else { rx=px; ry=py; dof=8;}                                                  //looking up or down. no hit  

  while(dof<8) 
  { 
   mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                     
   if(mp>0 && mp<mapX*mapY && map[mp]>0){ vmt=map[mp]-1; dof=8; disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
   else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
  } 
  vx=rx; vy=ry;

  //---Horizontal---
  dof=0; disH=100000;
  Tan=1.0/Tan; 
       if(sin(degToRad(ra))> 0.001){ ry=(((int)py>>6)<<6) -0.0001; rx=(py-ry)*Tan+px; yo=-64; xo=-yo*Tan;}//looking up 
  else if(sin(degToRad(ra))<-0.001){ ry=(((int)py>>6)<<6)+64;      rx=(py-ry)*Tan+px; yo= 64; xo=-yo*Tan;}//looking down
  else{ rx=px; ry=py; dof=8;}                                                   //looking straight left or right
 
  while(dof<8) 
  { 
   mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                          
   if(mp>0 && mp<mapX*mapY && map[mp]>0){ hmt=map[mp]-1; dof=8; disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
   else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
  } 
  
  float shade=1;
//   glColor3f(0,0.8,0);
  SDL_SetRenderDrawColor(renderer,0,255,0,0);
  if(disV<disH){ hmt=vmt; shade=0.5; rx=vx; ry=vy; disH=disV;   SDL_SetRenderDrawColor(renderer,0,200,0,0);;}//horizontal hit first
//   glLineWidth(2); glBegin(GL_LINES); glVertex2i(px,py); glVertex2i(rx,ry); glEnd();//draw 2D ray
    //  SDL_RenderDrawLine(renderer, px,py,rx,ry);
//   int ca=FixAng(pa-ra+0.5); disH=disH*cos(degToRad(ca));                            //fix fisheye 
  int lineH = (mapS*640)/(disH); 
  float ty_step=64.0/(float)lineH; 
  float ty_off=0; 
  if(lineH>640){ ty_off=(lineH-640)/2.0; lineH=640;}                            //line height and limit
  int lineOff = 320 - (lineH>>1);                                               //line offset

  //---draw walls---
  int y;
  float ty=ty_off*ty_step;
  float tx;
  if(shade==1){ tx=(int)(rx/2.0)%64; if(ra>180){ tx=31-tx;}}  
  else        { tx=(int)(ry/2.0)%64; if(ra>90 && ra<270){ tx=31-tx;}}
  for(y=0;y<lineH;y++)
  {
    int pixel=((int)ty*64+(int)tx)*3;
   int red   =all_textures(hmt,pixel+0)*shade;
   int green =all_textures(hmt,pixel+1)*shade;
   int blue  =all_textures(hmt,pixel+2)*shade;//door green
   drawPixel(renderer,8,8,red,green,blue, r*8,y+lineOff);//draw vertical wall  
//    if(hmt==0){ glColor3f(c    , c/2.0, c/2.0);} //checkerboard red
//    if(hmt==1){ glColor3f(c    , c    , c/2.0);} //Brick yellow
//    if(hmt==2){ glColor3f(c/2.0, c/2.0, c    );} //window blue
//    if(hmt==3){ glColor3f(c/2.0, c    , c/2.0);} //door green
//    glPointSize(8);glBegin(GL_POINTS);glVertex2i(r*8+530,y+lineOff);glEnd();//draw vertical wall  
   ty+=ty_step;
  }

//   // ------draw floors-----
//    for(y=lineOff+lineH;y<320;y++)
//  {
//   float dy=y-(320/2.0), deg=degToRad(ra), raFix=cos(degToRad(FixAng(pa-ra)));
//   tx=px/2 + cos(deg)*158*2*32/dy/raFix;
//   ty=py/2 - sin(deg)*158*2*32/dy/raFix;
//   int mp=mapF[(int)(ty/32.0)*mapX+(int)(tx/32.0)]*32*32;
//   int pixel=(((int)(py + ty*2)&63)*64 + ((int)(px + tx*2)&63))*3+mp*3;
// //   int pixel=(((int)(ty)&63)*64 + ((int)(tx)&63))*3+mp*3;
//   int red   =grass[pixel+0]*0.7;
//   int green =grass[pixel+1]*0.7;
//   int blue  =grass[pixel+2]*0.7;
//   drawPixel(renderer,8,8,red,green,blue,r*8,y);

//  //---draw ceiling---
// //   mp=mapC[(int)(ty/32.0)*mapX+(int)(tx/32.0)]*32*32;
// //   pixel=(((int)(ty)&31)*32 + ((int)(tx)&31))*3+mp*3;
// //   red   =texture1[pixel+0];
// //   green =texture1[pixel+1];
// //   blue  =texture1[pixel+2];
// //   if(mp>0){ drawPixel(renderer,8,8,red,green,blue,r*8,320-y);}
//  }
 
       ra=FixAng(ra-1); 
    }

}
void drawSky(SDL_Renderer *renderer)     //draw sky and rotate based on player rotation
{int x,y;
 for(y=0;y<40;y++)
 {
  for(x=0;x<120;x++)
  {
   int xo=(int)pa*2-x; if(xo<0){ xo+=120;} xo=xo % 120; //return 0-120 based on player angle
   int pixel=(y*120+xo)*3;
   int red   =sky[pixel+0];
   int green =sky[pixel+1];
   int blue  =sky[pixel+2];
   drawPixel(renderer,8,8,red,green,blue, x*8+530,y*8);
  }	
 }
}

void display(SDL_Renderer *renderer)
{
    frame2=SDL_GetTicks64(); fps=(frame2-frame1); frame1=SDL_GetTicks64(); 
//  drawMap2D(renderer);
 drawSky(renderer);
//  drawPlayer2D(renderer);
 drawRays2D(renderer);

}

void init(SDL_Renderer *renderer, SDL_Event event)
{
    // fps = 25;
    px=150; py=400; pa=90;
    int close_game = 0;
    pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));
    while (close_game == 0) {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (buttons(event) == -1)
                close_game = 1;
                break;
        }
        SDL_SetRenderDrawColor(renderer,70,70,70,0);
        SDL_RenderClear(renderer);
        display(renderer);
        
        SDL_RenderPresent(renderer);
    }
}
