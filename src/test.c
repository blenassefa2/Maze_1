/*
Test.c :- used to test my knowledge on SDL and raycasting
*/

#include "helper.h"


/**
* main - the entry funciton to the program
* @argc: number of command line arguments
* @argv: string array of command line arguments
* Return: (0) when successfull else (1)
*/
int main(int argc, char **argv)
{

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    int i;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_Surface* surface = IMG_Load("../resources/keys.png");
    SDL_Texture* wallTex = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("../resources/stats.png");
    SDL_Texture* statsTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    init(renderer,event, wallTex, statsTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

