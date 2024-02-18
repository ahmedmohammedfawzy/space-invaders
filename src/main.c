#include "entity.h"
#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdbool.h>
#include <stdio.h>

#define SHIP_SPEED 10

typedef struct State {
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Texture* texture;
    struct { 
        i32 x, y; 
    } ship;
    bool gameRunning;
} State;

SDL_Texture* loadTexture(State* state, const char* filePath) 
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(state->renderer, filePath);

    if (texture == NULL)
    {
        printf("Failed to load texture. %s", SDL_GetError());
    }

    return texture;
}

void renderToWindow(State* state, Entity* entity) 
{
    SDL_Rect src;
    src.x = entity->currentFrame.x;
    src.y = entity->currentFrame.y;
    src.w = entity->currentFrame.w;
    src.h = entity->currentFrame.h;

    SDL_Rect dst;
    dst.x = entity->x;
    dst.y = entity->y;
    dst.w = entity->currentFrame.w;
    dst.h = entity->currentFrame.w;

    SDL_RenderCopy(state->renderer, entity->texture, &src, &dst);
}

void processInput(State* state) 
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            case SDL_QUIT:
                state->gameRunning = false;
                break;

            default:
                break;
        }
    }
}

int main(void)
{
    // Initializing game objects
    if (SDL_Init(SDL_INIT_VIDEO)) 
    {
        printf("Could not init sdl %s", SDL_GetError());
    }

    if (!IMG_Init(IMG_INIT_PNG)) 
    {
        printf("Could not init sdl image %s", SDL_GetError());
    }

    State state = {0};
    state.gameRunning = true;

    state.window = SDL_CreateWindow("Space Invaders", -1, -1, 1280, 720, SDL_WINDOW_SHOWN);
    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);

    state.texture = 
        SDL_CreateTexture(
                state.renderer, 
                SDL_PIXELFORMAT_ARGB8888, 
                SDL_TEXTUREACCESS_TARGET, 256, 144);

    // Game main loop
    while (state.gameRunning)
    {
        processInput(&state);

        // drawing first on a texture before drawing to the screen
        SDL_SetRenderTarget(state.renderer, state.texture);
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state.renderer);

        SDL_Rect rect;
        rect.h = 16;
        rect.w = 16;
        rect.x = state.ship.x;
        rect.y = state.ship.y;
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(state.renderer, &rect);

        // transfering the texture to window backbuffer
        SDL_SetRenderTarget(state.renderer, NULL);
        SDL_RenderCopyEx(state.renderer, state.texture, NULL, NULL, 0, NULL, SDL_FLIP_VERTICAL);
        SDL_RenderPresent(state.renderer);
    }

    SDL_DestroyWindow(state.window);
    SDL_Quit();

    return 0;
}

