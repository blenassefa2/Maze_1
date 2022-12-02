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
    

    init(renderer,event);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

