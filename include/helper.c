
#include "helper.h"
#include "helper2.h"
void drawPixel(SDL_Renderer *renderer,int size, int r, int g, int b, int x, int y)
{
    SDL_Rect pixel = {x, y, size, size};
  
    SDL_SetRenderDrawColor(renderer,r,g,b,0);
    SDL_RenderDrawRect(renderer, &pixel);
    SDL_RenderFillRect(renderer,&pixel);
}

void drawSprite(SDL_Renderer *renderer)
{
    int x,y,s;
    if(px<sp[0].x+30 && px>sp[0].x-30 && py<sp[0].y+30 && py>sp[0].y-30){ sp[0].state=0;} //pick up key 	
    if(px<sp[3].x+30 && px>sp[3].x-30 && py<sp[3].y+30 && py>sp[3].y-30){ gameState=4;} //enemy kills

    //enemy attack
    int spx=(int)sp[3].x>>6,          spy=(int)sp[3].y>>6;          //normal grid position
    int spx_add=((int)sp[3].x+15)>>6, spy_add=((int)sp[3].y+15)>>6; //normal grid position plus     offset
    int spx_sub=((int)sp[3].x-15)>>6, spy_sub=((int)sp[3].y-15)>>6; //normal grid position subtract offset
    if(sp[3].x>px && map[spy*8+spx_sub]==0){ sp[3].x-=0.04*fps;}
    if(sp[3].x<px && map[spy*8+spx_add]==0){ sp[3].x+=0.04*fps;}
    if(sp[3].y>py && map[spy_sub*8+spx]==0){ sp[3].y-=0.04*fps;}
    if(sp[3].y<py && map[spy_add*8+spx]==0){ sp[3].y+=0.04*fps;}

    for(s=0;s<4;s++)
    {
        float sx=sp[s].x-px; //temp float variables
        float sy=sp[s].y-py;
        float sz=sp[s].z;

        float CS=cos(degToRad(pa)), SN=sin(degToRad(pa)); //rotate around origin
        float a=sy*CS+sx*SN; 
        float b=sx*CS-sy*SN; 
        sx=a; sy=b;

        sx=(sx*108.0/sy)+(120/2); //convert to screen x,y
        sy=(sz*108.0/sy)+( 80/2);

        int scale=32*80/b;   //scale sprite based on distance
        if(scale<0){ scale=0;} if(scale>120){ scale=120;}  

        //texture
        float t_x=0, t_y=31, t_x_step=31.5/(float)scale, t_y_step=32.0/(float)scale;

        for(x=sx-scale/2;x<sx+scale/2;x++)
        {
            t_y=31;
            for(y=0;y<scale;y++)
            {
                if(sp[s].state==1 && x>0 && x<120 && b<depth[x])
                {
                    int pixel=((int)t_y*32+(int)t_x)*3+(sp[s].map*32*32*3);
                    int red   =sprites[pixel+0];
                    int green =sprites[pixel+1];
                    int blue  =sprites[pixel+2];
                    drawPixel(renderer, 8, red, green, blue, x*8,sy*8-y*8);
                    t_y-=t_y_step; if(t_y<0){ t_y=0;}
                }
            }
            t_x+=t_x_step;
        }
    }
}

void drawMap2D(SDL_Renderer *renderer)
{
 int x, y, xo, yo;
 SDL_Rect tile = {0,0,mapS,mapS};

 for(y=0;y<mapY;y++)
 {
  for(x=0;x<mapX;x++)
  {
    if(map[y*mapX+x]>0)
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

void drawPlayer2D(SDL_Renderer *renderer)
{
    drawPixel(renderer, 8, px, py, 255, 255, 255);
    SDL_RenderDrawLine(renderer, px,py,px+pdx*20,py+pdy*20);
}

int buttons(SDL_Event event,int x,int y)
{
    int xo=0; if(pdx<0){ xo=-25;} else{ xo=25;}
    int yo=0; if(pdy<0){ yo=-25;} else{ yo=25;} 
    int ipx=px/64.0, ipx_add_xo=(px+xo)/64.0;
    int ipy=py/64.0, ipy_add_yo=(py+yo)/64.0;

    int pxo=0; if(pdx<0){ pxo=-20;} else{ pxo=20;}                                    //x offset to check map
    int pyo=0; if(pdy<0){ pyo=-20;} else{ pyo=20;}                                    //y offset to check map
    int pipx=px/32.0, pipx_add_xo=(px+pxo)/32.0, pipx_sub_xo=(px-pxo)/32.0;             //x position and offset
    int pipy=py/32.0, pipy_add_yo=(py+pyo)/32.0, pipy_sub_yo=(py-pyo)/32.0;             //y position and offset
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
                    if(map[pipy*mapX        + pipx_add_xo]==0){ px+=pdx*0.2*fps;}
                    if(map[pipy_add_yo*mapX + pipx       ]==0){ py+=pdy*0.2*fps;}
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    pa+=0.2*fps; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if(map[pipy*mapX        + pipx_sub_xo]==0){ px-=pdx*0.2*fps;}
                    if(map[pipy_sub_yo*mapX + pipx       ]==0){ py-=pdy*0.2*fps;}
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    pa-=0.2*fps; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));
                    break;
                case SDL_SCANCODE_E: 
                    
                    if(map[ipy_add_yo*mapX+ipx_add_xo]==4){ map[ipy_add_yo*mapX+ipx_add_xo]=0;}
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return 0;

}//-----------------------------------------------------------------------------

void drawRays2D(SDL_Renderer *renderer)
{  
    
    int r,mx,my,mp,dof,side; float vx,vy,rx,ry,ra,xo,yo,disV,disH; 
    // printf("problem is before vertical");
    ra=FixAng(pa+30);

    for (r = 0; r < 120; r++)
    {
        // printf("problem is on vertical");
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
  SDL_SetRenderDrawColor(renderer,0,200,0,0);
  if(disV<disH){ hmt=vmt; shade=0.5; rx=vx; ry=vy; disH=disV;SDL_SetRenderDrawColor(renderer,0,100,0,0);}//horizontal hit first
    
  int ca=FixAng(pa-ra); disH=disH*cos(degToRad(ca));                            //fix fisheye 
  int lineH = (mapS*640)/(disH); 
  float ty_step=32.0/(float)lineH; 
  float ty_off=0; 
  if(lineH>640){ ty_off=(lineH-640)/2.0; lineH=640;}                            //line height and limit
  int lineOff = 320 - (lineH>>1);                                               //line offset

  depth[r]=disH; //save this line's depth

//   printf("problem is before walls");
  //---draw walls---
  int y;
  float ty=ty_off*ty_step;//+hmt*32;
  float tx;
  if(shade==1){ tx=(int)(rx/2.0)%64; if(ra>180){ tx=63-tx;}}  
  else        { tx=(int)(ry/2.0)%64; if(ra>90 && ra<270){ tx=63-tx;}}
  for(y=0;y<lineH;y++)
  {
   int pixel=((int)ty*64+(int)tx)*3+(hmt*64*64*3);
   int red   =All_Textures[pixel+0]*shade;
   int green =All_Textures[pixel+1]*shade;
   int blue  =All_Textures[pixel+2]*shade;
   drawPixel(renderer, 8, red,green,blue,r*8,y+lineOff);
   ty+=ty_step;
  }
 
//  printf("problem is before floors");
  //---draw floors---
 for(y=lineOff+lineH;y<640;y++)
 {
  float dy=y-(640/2.0), deg=degToRad(ra), raFix=cos(degToRad(FixAng(pa-ra)));
  tx=px/2 + cos(deg)*158*2*32/dy/raFix;
  ty=py/2 - sin(deg)*158*2*32/dy/raFix;
  int mp=mapF[(int)(ty/32.0)*mapX+(int)(tx/32.0)]*32*32;
  int pixel=(((int)(ty)&31)*32 + ((int)(tx)&31))*3+mp*3;
  int red   =100;//All_Textures[pixel+0]*0.7;
  int green =200;//All_Textures[pixel+1]*0.7;
  int blue  =100;//All_Textures[pixel+2]*0.7;
  drawPixel(renderer, 8, red,green,blue,r*8,y);
        
    }
    ra=FixAng(ra-1);
       
    }
    // printf("no problem so far");
}

void drawSky(SDL_Renderer *renderer)     //draw sky and rotate based on player rotation
{
    int x,y;
    for(y=0;y<40;y++)
    {
        for(x=0;x<120;x++)
        {
            int xo=(int)pa*2-x; if(xo<0){ xo+=120;} xo=xo % 120; //return 0-120 based on player angle
            int pixel=(y*120+xo)*3;
            int red   =sky[pixel+0];
            int green =sky[pixel+1];
            int blue  =sky[pixel+2];
            drawPixel(renderer, 8,red,green,blue, x*8,y*8);
        }	
    }
}

void screen(SDL_Renderer *renderer,int v) //draw any full screen image. 120x80 pixels
{
 int x,y;
 int *T;
 if(v==1){ T=lost;}
 if(v==2){ T=won;}
 if(v==3){ T=lost;}
 for(y=0;y<80;y++)
 {
  for(x=0;x<120;x++)
  {
   int pixel=(y*120+x)*3;
   int red   =T[pixel+0]*fade;
   int green =T[pixel+1]*fade;
   int blue  =T[pixel+2]*fade;
   drawPixel(renderer, 8, red,green,blue,x*8,y*8);
  }	
 }	
 if(fade<1){ fade+=0.001*fps;} 
 if(fade>1){ fade=1;}
}


void display(SDL_Renderer *renderer)
{  
//  frame2=SDL_GetTicks64(); fps=(frame2-frame1); frame1=SDL_GetTicks64(); 

//  if(gameState==0){ init(); fade=0; timer=0; gameState=1;} //init game
//  if(gameState==1){ screen(renderer, 1); timer+=1*fps; if(timer>2000){ fade=0; timer=0; gameState=2;}} //start screen
 
//  if(gameState==2) //The main game loop
//  {
    
    // buttons
    drawSky(renderer);
    // printf("this works");
    drawRays2D(renderer);
    // drawSprite(renderer);

    // if(Keys.a==1)	
    // if(Keys.d==1){ pa-=0.2*fps; pa=FixAng(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 

    // int xo=0; if(pdx<0){ xo=-20;} else{ xo=20;}                                    //x offset to check map
    // int yo=0; if(pdy<0){ yo=-20;} else{ yo=20;}                                    //y offset to check map
    // int ipx=px/32.0, ipx_add_xo=(px+xo)/32.0, ipx_sub_xo=(px-xo)/32.0;             //x position and offset
    // int ipy=py/32.0, ipy_add_yo=(py+yo)/32.0, ipy_sub_yo=(py-yo)/32.0;             //y position and offset
    // if(Keys.w==1)                                                                  //move forward
    // {  
    // if(map[ipy*mapX        + ipx_add_xo]==0){ px+=pdx*0.2*fps;}
    // if(map[ipy_add_yo*mapX + ipx       ]==0){ py+=pdy*0.2*fps;}
    // // }
    // if(Keys.s==1)                                                                  //move backward
    // { 
    // if(map[ipy*mapX        + ipx_sub_xo]==0){ px-=pdx*0.2*fps;}
    // if(map[ipy_sub_yo*mapX + ipx       ]==0){ py-=pdy*0.2*fps;}
    // } 
    
    // if( (int)px>>6==1 && (int)py>>6==1 ){ fade=0; timer=0; gameState=3;} //Entered block 1, Win game!!
// }

//  if(gameState==3){ screen(renderer, 2); timer+=1*fps; if(timer>2000){ fade=0; timer=0; event.type =SDL_QUIT;}} //won screen
//  if(gameState==4){ screen(renderer, 3); timer+=1*fps; if(timer>2000){ fade=0; timer=0; event.type = SDL_QUIT;}} //lost screen

}


void init(SDL_Renderer *renderer, SDL_Event event)
{
    px=150; py=400; pa=90;
    int close_game = 0;
    map[19]= 4; map[26] = 4;
    pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));

    sp[0].type=1; sp[0].state=1; sp[0].map=0; sp[0].x=1.5*64; sp[0].y=5*64;   sp[0].z=20; //key
    sp[1].type=2; sp[1].state=1; sp[1].map=1; sp[1].x=1.5*64; sp[1].y=4.5*64; sp[1].z= 0; //light 1
    sp[2].type=2; sp[2].state=1; sp[2].map=1; sp[2].x=3.5*64; sp[2].y=4.5*64; sp[2].z= 0; //light 2
    sp[3].type=3; sp[3].state=1; sp[3].map=2; sp[3].x=2.5*64; sp[3].y=2*64;   sp[3].z=20; //enemy
     

    while (close_game == 0) {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (buttons(event,0,0) == -1)
                close_game = 1;
                break;
        }
        SDL_SetRenderDrawColor(renderer,70,70,70,0);
        // SDL_RenderClear(renderer);
        frame2=SDL_GetTicks64(); fps=(frame2-frame1); frame1=SDL_GetTicks64(); 

        if (gameState == 1) {screen(renderer, 1); timer+=1*fps; if(timer>2000){ fade=0; timer=0; gameState=2;}}
        // drawSky(renderer);
        // drawRays2D(renderer);
        // drawSprite(renderer);
        display(renderer);
        
        SDL_RenderPresent(renderer);
    }
}
