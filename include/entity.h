#pragma once
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

typedef struct Entity {
  float x, y;
  SDL_Rect currentFrame;
  SDL_Texture* texture;
} Entity;

void Entity_Init(Entity* entity, float x, float y, SDL_Texture* tex);
