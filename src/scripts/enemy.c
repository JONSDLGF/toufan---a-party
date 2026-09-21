#include "enemy.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void enemy_init(Character *enemy) {
    enemy->x = SCREEN_W / 2.0f;
    enemy->y = SCREEN_H * 0.2f;
    enemy->radius = 20.0f;
    enemy->lives = 20;
    enemy->shot_timer = 0;
}

/* Fires a ring of ENEMY_RING_COUNT bullets, with one of them always
 * aimed exactly at (target_x, target_y). This replaces the old
 * hardcoded vx=1, vy=1 -- every enemy bullet used to fly the same
 * fixed direction forever. */
static void fire_pattern(Character *enemy, float target_x, float target_y) {
    float base_angle = atan2f(target_y - enemy->y, target_x - enemy->x);

    for (int i = 0; i < ENEMY_RING_COUNT; i++) {
        float angle = base_angle + (2.0f * (float)M_PI * i) / ENEMY_RING_COUNT;
        float vx = cosf(angle) * ENEMY_BULLET_SPEED;
        float vy = sinf(angle) * ENEMY_BULLET_SPEED;
        bullet_spawn(enemy->x, enemy->y, vx, vy, 6.0f, true);
    }
}

void enemy_update(Character *enemy, float target_x, float target_y) {
    if (enemy->shot_timer == 0) {
        fire_pattern(enemy, target_x, target_y);
        enemy->shot_timer = ENEMY_FIRE_INTERVAL;
        return;
    }
    enemy->shot_timer--;
}

void enemy_draw(const Character *enemy, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 80, 120, 255, 255);
    int r = (int)enemy->radius;
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x * x + y * y <= r * r) {
                SDL_RenderDrawPoint(renderer, (int)enemy->x + x, (int)enemy->y + y);
            }
        }
    }
}
