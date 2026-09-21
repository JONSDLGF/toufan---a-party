#ifndef ENEMY_H
#define ENEMY_H

#include "../main.h"
#include "bullet.h"

#define ENEMY_FIRE_INTERVAL 45 /* frames between shots */
#define ENEMY_BULLET_SPEED  2.5f
#define ENEMY_RING_COUNT    8

void enemy_init(Character *enemy);
/* target_x/y: usually the player's position, used to aim. */
void enemy_update(Character *enemy, float target_x, float target_y);
void enemy_draw(const Character *enemy, SDL_Renderer *renderer);

#endif
