#ifndef PLAYER_H
#define PLAYER_H

#include "../main.h"
#include "bullet.h"

#define PLAYER_SPEED       4.0f
#define PLAYER_SPEED_FOCUS 2.0f
#define PLAYER_SHOT_COOLDOWN 8 /* frames between shots */

void player_init(Character *player);
void player_update(Character *player, const Input *input);
void player_draw(const Character *player, SDL_Renderer *renderer);

#endif
