
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
    if(map[y*mapX+x]==1)
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
 SDL_Rect player = {px, py, 8, 8};
  

 
 SDL_SetRenderDrawColor(renderer,255,255,0,255);
 SDL_RenderDrawRect(renderer, &player);
 SDL_RenderFillRect(renderer,&player);

 SDL_RenderDrawLine(renderer, px,py,px+pdx*5,py+pdy*5);
}

int buttons(SDL_Event event,int x,int y)
{
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
                    px+=pdx; py+=pdy;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    pa -= 0.1;if (pa<0){pa+=2*PI;}pdx=cos(pa)*(5);pdy=sin(pa)*5;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    px-=pdx; py-=pdy;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    pa += 0.1;if (pa>2*PI){pa-=2*PI;}pdx=cos(pa)*(5);pdy=sin(pa)*5;
                    break;
            }
        default:
            break;
    }
    return 0;

}//-----------------------------------------------------------------------------

void drawRays2D(SDL_Renderer *renderer)
{
    int r,mx,my,mp,dof; float rx,ry,ra,xo,yo,disT;

    ra=pa-DR*30; if (ra<0){ra+=2*PI;}if(ra>2*PI){ra-=2*PI;}

    for (r = 0; r < 70; r++)
    {
        // horizontal lines
        dof=0;
        float disH= 1000000,hx=px,hy=py;
        float aTan = -1/tan(ra);
        if (ra>PI){ry=(((int)py>>6)<<6)-0.0001; rx=(py-ry)*aTan+px;yo=-64;xo=-yo*aTan;}
        if (ra<PI){ry=(((int)py>>6)<<6)+64;     rx=(py-ry)*aTan+px;yo= 64;xo=-yo*aTan;}
        if (ra==0 || ra==PI){ rx=px; ry=py; dof=8;}

        while(dof<8)
        {
            mx= (int)(rx)>>6; my = (int)(ry)>>6; mp=my*mapX+mx;
            if (mp>0 && mp<mapX*mapY && map[mp] == 1){hx=rx; hy=ry; disH=distance(px,py,hx,hy,ra); dof=8;}
            else{rx+=xo; ry+= yo; dof += 1;}
        }
    
        // vertical lines
        dof=0;
        float disV= 1000000,vx=px,vy=py;
        float nTan = -tan(ra);
        if (ra>P2 && ra<P3){rx=(((int)px>>6)<<6)-0.0001; ry=(px-rx)*nTan+py;xo=-64;yo=-xo*nTan;}
        if (ra<P2 || ra>P3){rx=(((int)px>>6)<<6)+64;     ry=(px-rx)*nTan+py;xo= 64;yo=-xo*nTan;}
        if (ra==0 || ra==PI){ rx=px; ry=py; dof=8;}

        while(dof<8)
        {
            mx= (int)(rx)>>6; my = (int)(ry)>>6; mp=my*mapX+mx;
            if (mp>0 && mp<mapX*mapY && map[mp] == 1){ vx=rx; vy=ry; disV=distance(px,py,vx,vy,ra); dof=8;}
            else{rx+=xo; ry+= yo; dof += 1;}
        }
        if (disV<disH) {rx=vx;ry=vy;disT=disV; SDL_SetRenderDrawColor(renderer,250,0,0,0);}
        if (disV>disH) {rx=hx;ry=hy;disT=disH;SDL_SetRenderDrawColor(renderer,200,0,0,0);}
        SDL_RenderDrawLine(renderer, px,py,rx,ry);
        
        //draw the 3D walls
        float ca=pa-ra;if (ca<0){ca+=2*PI;}if(ca>2*PI){ca-=2*PI;} disT=disT*cos(ca);
        float lineH = (mapS*320)/disT;if(lineH>320){lineH=320;}
        float lineO = 160-lineH/2;
        
    
        SDL_Rect wall = {r*8+530, lineO, 8, lineH};
      
        SDL_RenderDrawRect(renderer, &wall);
        SDL_RenderFillRect(renderer,&wall);

        ra+=DR; if (ra<0){ra+=2*PI;}if(ra>2*PI){ra-=2*PI;}
    }

}

void display(SDL_Renderer *renderer)
{   
 drawMap2D(renderer);
 drawPlayer2D(renderer);
 drawRays2D(renderer);

}

void init(SDL_Renderer *renderer, SDL_Event event)
{
    px=150; py=400; pa=90;
    int close_game = 0;
    pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));
    while (close_game == 0) {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (buttons(event,0,0) == -1)
                close_game = 1;
                break;
        }
        SDL_SetRenderDrawColor(renderer,70,70,70,0);
        SDL_RenderClear(renderer);
        display(renderer);
        
        SDL_RenderPresent(renderer);
    }
}
