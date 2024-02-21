#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdbool.h>
#include <stdio.h>

#define SHIP_SPEED 3
#define SPRITE_SIZE 16

typedef struct Vec2i {
    i32 x,y;
} Vec2i;

typedef struct Vec2f {
    f32 x,y;
} Vec2f;

typedef struct State {
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Texture* texture;
    SDL_Texture* sprites;
    struct { 
        i32 x, y; 
    } ship;

    struct {
        bool left, right;
    } keyboard;

    bool gameRunning;
} State;


void drawSprite(State* state, Vec2i index, Vec2f pos) 
{
    SDL_Rect src;
    src.w = SPRITE_SIZE;
    src.h = SPRITE_SIZE;
    src.x = SPRITE_SIZE * index.x;
    src.y = SPRITE_SIZE * index.y;

    SDL_Rect dst;
    dst.w = SPRITE_SIZE;
    dst.h = SPRITE_SIZE;
    dst.x = pos.x;
    dst.y = pos.y;

    // flipping the sprite because i could not manage to flip at load
    SDL_RenderCopyEx(
            state->renderer, state->sprites, &src, &dst, 0, NULL, SDL_FLIP_VERTICAL);
}


void processInput(State* state) 
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ) {
                    case SDLK_LEFT:
                        state->keyboard.left = true;
                        break;
                    case SDLK_RIGHT:
                        state->keyboard.right = true;
                        break;
                }
                break;

            case SDL_KEYUP:
                switch( event.key.keysym.sym ) {
                    case SDLK_LEFT:
                        state->keyboard.left = false;
                        break;
                    case SDLK_RIGHT:
                        state->keyboard.right = false;
                        break;
                }
                break;

            case SDL_QUIT:
                state->gameRunning = false;
                break;

            default:
                break;
        }
    }
}

void update(State* state) 
{
    if (state->keyboard.left == true) 
    {
        state->ship.x -= SHIP_SPEED;
    }

    if (state->keyboard.right == true) 
    {
        state->ship.x += SHIP_SPEED;
    }
}

void render(State* state) 
{
    // drawing first on a texture before drawing to the screen
    SDL_SetRenderTarget(state->renderer, state->texture);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    // drawing sprites
    drawSprite(state, (Vec2i){0, 0}, (Vec2f){state->ship.x, state->ship.y});

    // transfering the texture to window backbuffer
    SDL_SetRenderTarget(state->renderer, NULL);
    SDL_RenderCopyEx(state->renderer, state->texture, NULL, NULL, 0, NULL, SDL_FLIP_VERTICAL);
    SDL_RenderPresent(state->renderer);
}

int main(void)
{
    // Initializing SDL
    if (SDL_Init(SDL_INIT_VIDEO)) 
    {
        printf("Could not init sdl %s", SDL_GetError());
    }

    if (!IMG_Init(IMG_INIT_PNG)) 
    {
        printf("Could not init sdl image %s", SDL_GetError());
    }

    // Initializing game objects
    State state = {0};
    state.gameRunning = true;

    state.window = SDL_CreateWindow("Space Invaders", -1, -1, 1280, 720, SDL_WINDOW_SHOWN);
    state.renderer = SDL_CreateRenderer(
            state.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    state.texture = 
        SDL_CreateTexture(
                state.renderer, 
                SDL_PIXELFORMAT_ARGB8888, 
                SDL_TEXTUREACCESS_TARGET, 256, 144);

    // loading sprites and creating texture
    state.sprites = IMG_LoadTexture(state.renderer, "res/sprites.png");

    // Game main loop
    while (state.gameRunning)
    {
        processInput(&state);
        update(&state);
        render(&state);
    }

    SDL_DestroyTexture(state.sprites);
    SDL_DestroyWindow(state.window);
    SDL_Quit();

    return 0;
}

