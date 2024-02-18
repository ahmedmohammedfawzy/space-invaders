#include "entity.h"

void Entity_Init(Entity* entity, float x, float y, SDL_Texture* tex) 
{
    entity->x = x;
    entity->y = y;

    entity->currentFrame.x = 0;
    entity->currentFrame.y = 0;
    entity->currentFrame.w = 32;
    entity->currentFrame.h = 32;

    entity->texture = tex;
}
