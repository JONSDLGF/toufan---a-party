#ifndef BULLET_H
#define BULLET_H

#include "../main.h"

#define MAX_BULLETS 2048

/* Wall-touch flags, returned by bullet_wall_flags(). Named distinctly
 * from InputFlags in main.h so "key & X" and "wall & X" are never the
 * same constant. */
typedef enum {
    WALL_UP    = 1 << 0,
    WALL_DOWN  = 1 << 1,
    WALL_LEFT  = 1 << 2,
    WALL_RIGHT = 1 << 3
} WallFlags;

typedef struct {
    float x, y;
    float vx, vy;
    float radius;
    bool  enemy;   /* true = belongs to an enemy, hurts the player */
    bool  active;
} Bull;

typedef struct {
    float x, y;
    float radius; /* hitbox radius, smaller than the sprite radius */
    int   lives;
    int   shot_timer; /* frames until this character may fire again */
} Character;

/* Shared bullet pool. One pool, split by the `enemy` flag, rather than
 * two separate arrays -- simplest for now; revisit if enemy bullet
 * counts grow much larger than player bullet counts and you want
 * different pool sizes. */
extern Bull g_bullets[MAX_BULLETS];

void  bullet_pool_reset(void);
bool  bullet_spawn(float x, float y, float vx, float vy, float radius, bool enemy);
void  bullets_update(void);
void  bullets_cull_offscreen(void);
/* Resolves collisions between `c` and every active bullet whose
 * `enemy` flag differs from want_enemy_flag (i.e. bullets that can
 * hurt `c`). Decrements c->lives and deactivates hit bullets. */
void  bullets_collide_character(Character *c, bool want_enemy_flag);
int   bullet_wall_flags(float x, float y, float radius);

void  bullets_draw(SDL_Renderer *renderer, SDL_Texture *circle_tex);

#endif
