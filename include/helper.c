
#include "helper.h"
#include "helper2.h"

void drawMap2D()
{
    int x, y, xo, yo;
    SDL_Rect tile = {0, 0, mapS/2, mapS/2};

    for(y = 0; y < mapY; y++)
    {
        for(x = 0; x < mapX; x++)
        {
            if(map[y * mapX + x] >= 1)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 

            xo = x * (mapS / 2 + 1); yo = y * (mapS / 2 + 1);
            tile.x = xo, tile.y = yo;
            SDL_RenderDrawRect(renderer, &tile);
            SDL_RenderFillRect(renderer, &tile);
        } 
    }
}

void drawPixel(int width, int height, int r, int g, int b, int x, int y)
{
    SDL_Rect pixel = {x, y, width, height};
    
    SDL_SetRenderDrawColor(renderer, r, g, b, 0);
    SDL_RenderDrawRect(renderer, &pixel);
    SDL_RenderFillRect(renderer, &pixel);
}

void drawPlayer2D()
{
    drawPixel(8, 8, 255, 255, 0, px / 2, py / 2);
}

int isDoor() 
{
    int xo = 0, yo = 0, addX = 0, addY = 0;

    if (pdx < 0)
        xo = -25; 
    else
        xo = 25;

    
    if (pdy < 0)
        yo = -25;
    else
        yo = 25;

    addX =(px + xo) / 64.0, addY = (py + yo) / 64.0;

    if(map[addY * mapX + addX] == 3)
        return addY * mapX + addX;
    
    return -1;
}

int buttons(SDL_Event event)
{
    int door = -1;
    int xo = 0, yo = 0, ipx = px / 64.0, ipy = py / 64.0;
    int ipx_add_xo, ipx_sub_xo;
    int ipy_add_yo, ipy_sub_yo;
    
    //x offset to check map
    if(pdx < 0)
        xo = -20;
    else
        xo = 20;   
    
    //y offset to check map       
    if(pdy < 0)
        yo = -20;
    else
        yo = 20;    
    
    //positions and offset
    ipx_add_xo = (px + xo) / 64.0, ipx_sub_xo = (px - xo) / 64.0;     
    ipy_add_yo = (py + yo) / 64.0, ipy_sub_yo = (py - yo) / 64.0; 

    switch (event.type)
    {
        case SDL_QUIT:
            return -1;
        case SDL_KEYUP:
            if (event.key.keysym.scancode == SDL_SCANCODE_M)
                drawMap = 1;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:

                    if(map[ipy * mapX + ipx_add_xo] == 0)
                        px += pdx * 0.2 * fps;

                    if(map[ipy_add_yo * mapX + ipx] == 0)
                        py += pdy * 0.2 * fps;
                    break;

                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:

                    pa += 0.2 * fps, pa = FixAng(pa);
                    pdx = cos(degToRad(pa)), pdy = -sin(degToRad(pa));
                    break;

                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if(map[ipy * mapX + ipx_sub_xo] == 0)
                        px -= pdy * 0.2 * fps;
                    if(map[ipy_sub_yo * mapX + ipx] == 0)
                        py -= pdy * 0.2 * fps;
                    break;

                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    pa -= 0.2 * fps, pa=FixAng(pa);
                    pdx = cos(degToRad(pa)), pdy = -sin(degToRad(pa));
                    break;

                case SDL_SCANCODE_E:
                    door  = isDoor();
                    if (door > -1 && keyCount > 0)
                        map[door] = 0, keyCount -= 1;
                    break;

                case SDL_SCANCODE_M:
                    drawMap =0;
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

int allTextures(int hmt, int pixel)
{
    switch (hmt)
    {
        case 0:
            return texture1[pixel];
        case 1:
            return texture2[pixel];
        case 2:
            if (door[pixel] == 0)
                return texture1[pixel];
            return door[pixel];
        case 3:
            if (finish[pixel] == 0)
                return texture1[pixel];
            return finish[pixel];
        case 4:
            return key[pixel];
        case 5:
            return diamond[pixel];
        case 6:
            return knight[pixel];

        default:
            break;
    }
    return 0;
}

void drawRays2D()
{
    int r, mx, my, mp, dof, side; 
    float vx, vy, rx, ry, ra, xo, yo, disV, disH, Tan, shade = 1; 
    
    //ray set back 30 degrees
    ra = FixAng(pa + 30);                                                              
        
    for (r = 0; r < 120; r++)
    {
        
        // Vertical and horizontal map texture number 
        int vmt = 0, hmt = 0;          
        
        //---Vertical Rays--- 
        dof = 0, side = 0, disV = 100000;
        Tan = tan(degToRad(ra));
        
       if (cos(degToRad(ra))> 0.001)
        {
            rx = (((int) px >> 6) << 6) + 64;
            ry = (px - rx) * Tan + py;
            xo = 64; yo = -xo * Tan;
        }// Looking left
        else if(cos(degToRad(ra)) < -0.001)
        { 
            rx = (((int)px >> 6) << 6) - 0.0001, ry = (px - rx) * Tan + py; 
            xo=-64; yo=-xo*Tan;
        }// Looking right
        else    
        {
            rx = px, ry = py, dof = 8; 
        }// Looking up or down. no hit                                                //looking up or down. no hit  

        while (dof < 8) 
        { 
            mx = (int)(rx) >> 6, my = (int)(ry) >> 6, mp = my * mapX + mx;                     
            if(mp > 0 && mp < mapX * mapY && map[mp] > 0)//hit
            { 
                vmt = map[mp] - 1; dof = 8; 
                disV = cos(degToRad(ra)) * (rx - px) - sin(degToRad(ra)) * (ry - py);
            }         
            else
            {
                rx += xo, ry += yo, dof += 1;
            }//check next horizontal                                         
        } 
        
        vx = rx, vy = ry;

        //---Horizontal---
        dof = 0, disH = 100000;
        Tan = 1.0 / Tan; 
        if (sin(degToRad(ra)) > 0.001)
        { 
            ry = (((int)py >> 6) << 6) - 0.0001, rx = (py - ry) * Tan + px; 
            yo=-64; xo=-yo*Tan;
        }//looking up 
        else if (sin(degToRad(ra)) < -0.001)
        { 
            ry = (((int)py >> 6) << 6) + 64, rx = (py - ry) * Tan + px; 
            yo = 64, xo = -yo * Tan;
        }//looking down
        else
        { 
            rx = px, ry = py, dof = 8;
        }//looking straight left or right                                                   //looking straight left or right
 
        while(dof < 8) 
        { 
            mx = (int)(rx)>>6, my = (int)(ry)>>6,  mp = my * mapX + mx;                          
            if (mp > 0 && mp < mapX * mapY && map[mp] > 0)
            { 
                hmt = map[mp] - 1, dof = 8; 
                disH = cos(degToRad(ra)) * (rx - px) - sin(degToRad(ra)) * (ry - py);
            }//hit         
            else
            { 
                rx += xo, ry += yo, dof += 1;
            }//check next horizontal
        }  
  
        shade=1;
        // If Horizontal hit first
        if(disV<disH)
            hmt = vmt, shade = 0.5, rx = vx, ry = vy, disH = disV;
        
        //fix fisheye
        int ca = FixAng(pa - ra);
        disH = disH * cos(degToRad(ca)); 
        int lineH = (mapS * 640) / (disH); 
        float tyStep = 64.0 / (float)lineH; 
        float tyOff = 0;

         //line height and limit 
        if (lineH > 640)
            tyOff = (lineH - 640) / 2.0, lineH=640;
        
        //line offset
        int lineOff = 320 - (lineH>>1);                       
        
        //save this line's depth for sprites
        depth[r]=disH; 
       
        //---draw walls---
        int y;
        float ty = tyOff * tyStep, tx;
        
        if (shade == 1)
        { 
            tx = (int)(rx / 2.0) % 64;
            if(ra > 180)
                tx = 31 - tx;
        }  
        else
        { 
            tx = (int)(ry / 2.0) % 64; 
            if (ra > 90 && ra < 270) 
                tx = 31 - tx;
        }

        for (y = 0; y < lineH; y++)
        {
            int pixel=((int)ty * 64 + (int)tx) * 3;
            int red   = allTextures(hmt, pixel + 0) * shade;
            int green = allTextures(hmt, pixel + 1) * shade;
            int blue  = allTextures(hmt, pixel + 2) * shade;
            
            //draw vertical wall with each pixel 
            drawPixel(8, 8, red, green, blue, r * 8, y + lineOff);
        
            ty += tyStep;
        }

        // ------draw floors-----
        for (y = lineOff + lineH; y < 640; y++)
        {
            float dy = y - (640 / 2.0), deg = degToRad(ra), raFix = cos(degToRad(FixAng(pa - ra)));
            
            tx = px / 2 + cos(deg) * 158 * 2 * 32 / dy / raFix;
            ty = py / 2 - sin(deg) * 158 * 2 * 32 / dy / raFix;
            
            int pixel = (((int)(py + ty * 2) & 63) * 64 + ((int)(px + tx * 2) & 63)) * 3;
           
            int red   = grass[pixel + 0] * 0.7;
            int green = grass[pixel + 1] * 0.7;
            int blue  = grass[pixel + 2] * 0.7;
            drawPixel(8, 8, red, green, blue, r * 8, y);
        }
        ra = FixAng(ra-0.5); 
    
    } 

}

void pickKeys()
{
    int i;

    // Pick up each key 
    for (i = 0; i < 1; i++)
        if (
            sp[i].state == 1 
            && px<sp[i].x+30 
            && px>sp[i].x-30 
            && py<sp[i].y+30 
            && py>sp[i].y-30
        )
            keyCount += 1, sp[i].state=0;

}

void attackPlayer()
{
    int i;

    // Attacks by each enemy
    for (i = 3; i < 4; i++)
         if (
            px<sp[i].x+30 
            && px>sp[i].x-30 
            && py<sp[i].y+30 
            && py>sp[i].y-30
        )
        { 
            heartCount -= 1; 
            if (heartCount == 0)
                closeGame = 1;
            gameState=4;
        } //enemy kills

}

void moveEnemy()
{
    int i, spx, spy;
    int spxAdd, spyAdd, spxSub, spySub;

    // move each enemy
    for (i = 3; i < 4; i++)
    {
        //enemy attack
        spx = (int)sp[i].x >> 6, spy = (int)sp[i].y >> 6;                     //normal grid position
        spxAdd = ((int)sp[i].x + 15) >> 6, spyAdd = ((int)sp[i].y + 15) >> 6; //normal grid position plus     offset
        spxSub = ((int)sp[i].x - 15) >> 6, spySub = ((int)sp[i].y - 15) >> 6; //normal grid position subtract offset
        if (sp[i].x > px && map[spy * 8 + spxSub] == 0)
            sp[i].x -= 0.04 * fps;
        if (sp[i].x < px && map[spy * 8 + spxAdd] == 0)
            sp[i].x += 0.04 * fps;
        if (sp[i].y > py && map[spySub * 8 + spx] == 0)
            sp[i].y -= 0.04 * fps;
        if (sp[i].y < py && map[spyAdd * 8 + spx] == 0)
            sp[i].y += 0.04 * fps;
    }
}

void drawSprite()
{
    
    int x, y, s, scale;
    
    // Rotate around origin
    float sy, sx, sz, a, b;
    float textureX, textureY, textureXStep, textureYStep;
 
    float CS=cos(degToRad(pa)), SN=sin(degToRad(pa)); 

 	pickKeys();
    
    attackPlayer();
    moveEnemy();
    
    for (s = 0; s < 4; s++)
    {
        // Temporary float variables
        sx = sp[s].x - px, sy = sp[s].y - py, sz = sp[s].z;

        
        a = sy * CS + sx * SN; 
        b = sx * CS - sy * SN; 
        sx=a; sy=b;

        sx = (sx * 108.0 / sy) + (120 / 2); //convert to screen x,y
        sy = (sz * 108.0 / sy) + ( 80 / 2);

        scale = 32 * 80 / b;   //scale sprite based on distance
        
        if (scale < 0)
            scale = 0; 
        
        if(scale > 120)
            scale=120;  

        //texture
        textureX = 0, textureY = 31, textureXStep = 31.5 /(float)scale, textureYStep = 32.0/(float)scale;

        for (x = sx - scale / 2; x < sx + scale / 2; x++)
        {
            textureY = 31;
            for (y = 0; y < scale; y++)
            {
                if (sp[s].state == 1 && x > 0 && x < 120 && b < depth[x])
                {
                    
                    int pixel = ((int)textureY * 32 + (int)textureX) * 3;
                    int red   = allTextures(sp[s].map, pixel + 0);
                    int green = allTextures(sp[s].map, pixel + 1);
                    int blue  = allTextures(sp[s].map, pixel + 2);
                    
                    //dont draw if black
                    if(red!=0, green!=0, blue!=0) 
                        drawPixel(8, 8, red, green, blue, x*8,sy*8-y*8);
                    
                    textureY -= textureYStep;
                    if (textureY < 0)
                        textureY = 0;
                }
            }
            textureX += textureXStep;
        }
    }
}

void screen(int v) //draw any full screen image. 120x80 pixels
{
    int x, y, *T;

    if(v==1)
        T=title;
    if(v==2)
        T=won;
    if(v==3)
        T=lost;

    for(y = 0; y < 80; y++)
    {
        for(x = 0; x < 120; x++)
        {
            int pixel = (y * 120 + x) * 3;
            int red   = T[pixel + 0] * fade;
            int green = T[pixel + 1] * fade;
            int blue  = T[pixel + 2] * fade;
            drawPixel(8, 8, red, green, blue, x * 8, y * 8);
        }	
    }	
    if(fade<1)
        fade += 0.001 * fps; 
    if(fade>1)
        fade = 1;
}

void drawSky()     //draw sky and rotate based on player rotation
{
    int x, y;
    for(y = 0; y < 40; y++)
    {
        for(x = 0; x < 120; x++)
        {
            int xo = (int)pa * 2 - x; 
            if(xo<0)
                xo += 120; 
            
            //return 0-120 based on player angle
            xo = xo % 120; 
            int pixel = (y*120+xo)*3;
            int red   = sky[pixel+0];
            int green = sky[pixel+1];
            int blue  = sky[pixel+2];
            drawPixel(8, 8, red, green, blue, x*8, y*8);
        }	
    }
}

void drawStatistics(int x)
{
    // Stats board
    SDL_Rect stat = {x, 0, 300, 200};
    SDL_RenderCopy(renderer, stats, NULL, &stat);

    // health stats
    SDL_Rect score = {x + 65, 142, 15, 15};
    SDL_RenderCopy(renderer, scores[heartCount], NULL, &score);
    
    // diamond stats
    score.x =x +137;
    SDL_RenderCopy(renderer, scores[diamondCount], NULL, &score);
    
    // key stats
    score.x =x+212;
    SDL_RenderCopy(renderer, scores[keyCount], NULL, &score);
}

void display()
{
    // Adjust frames per second
    frame2 = SDL_GetTicks64(); 
    fps = (frame2-frame1); 
    frame1 = SDL_GetTicks64();

    // Initialize the game
    if(gameState == 0)
        fade=0, timer=0, gameState=1; 
    
    // Display title Screen and set gameState
    if(gameState==1)
    { 
        screen(1); 
        timer += 1*fps; 
        
        if(timer>2000)
            fade=0, timer=0, gameState=2;
    }
    
    // Start playing the game
    if (gameState==2)
    {
        
        drawSky(); 
        drawRays2D();
        drawSprite();

        // Adjust the map and stats 
        if (drawMap == 0)
        {
            drawStatistics( WINDOW_WIDTH - 300);
            drawMap2D();
            drawPlayer2D();
        }
        else
        {
            drawStatistics(0);
        }
    }
}

void loadImages()
{
    SDL_Surface* surface = IMG_Load("../resources/keys.png");
    keys =SDL_CreateTextureFromSurface(renderer, surface);

    surface = IMG_Load("../resources/stat.png");
    stats =  SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    SDL_Surface **surface1 = malloc(sizeof(SDL_Surface)* 5);
    surface1[0] = IMG_Load("../resources/zero.png");
    surface1[1] = IMG_Load("../resources/one.png");
    surface1[2]= IMG_Load("../resources/two.png");
    surface1[3] = IMG_Load("../resources/three.png");
    surface1[4]= IMG_Load("../resources/four.png");
    surface1[5]= IMG_Load("../resources/five.png");
    scores =  malloc(sizeof(SDL_Surface)* 6);

    for (int i = 0; i < 6; i++)
    {
        scores[i] = SDL_CreateTextureFromSurface(renderer, surface1[i]);
        SDL_FreeSurface(surface1[i]);
    }
    free(surface1);
}

void init()
{
    px=150; py=400; pa=90;
    
    keyCount=0, heartCount=5, diamondCount=0;

    sp[0].type=1; sp[0].state=1; sp[0].map=4; sp[0].x=1.5*64; sp[0].y=5*64;   sp[0].z=20; //key
    sp[1].type=2; sp[1].state=1; sp[1].map=5; sp[1].x=1.5*64; sp[1].y=4.5*64; sp[1].z= 20; //light 1
    sp[2].type=2; sp[2].state=1; sp[2].map=5; sp[2].x=4.5*64; sp[2].y=4.5*64; sp[2].z=20; //light 2
    sp[3].type=3; sp[3].state=1; sp[3].map=6; sp[3].x=2.5*64; sp[3].y=2*64;   sp[3].z=20; //enemy
    
    pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));
}

void setUp(SDL_Renderer *gameRenderer)
{
    renderer = gameRenderer;
    loadImages();
}

void startGame(SDL_Event event)
{
    init();
    while (closeGame == 0) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (buttons(event) == -1 )
                closeGame = 1;
                break;
        }
        SDL_RenderClear(renderer);
        
        display();
        SDL_RenderPresent(renderer);
    }
}