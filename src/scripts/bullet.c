#include "bullet.h"
#include <string.h>

Bull g_bullets[MAX_BULLETS];

void bullet_pool_reset(void) {
    memset(g_bullets, 0, sizeof(g_bullets));
}

bool bullet_spawn(float x, float y, float vx, float vy, float radius, bool enemy) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!g_bullets[i].active) {
            g_bullets[i].x = x;
            g_bullets[i].y = y;
            g_bullets[i].vx = vx;
            g_bullets[i].vy = vy;
            g_bullets[i].radius = radius;
            g_bullets[i].enemy = enemy;
            g_bullets[i].active = true;
            return true;
        }
    }
    return false; /* pool full; caller may choose to log/ignore */
}

void bullets_update(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!g_bullets[i].active) continue;
        g_bullets[i].x += g_bullets[i].vx;
        g_bullets[i].y += g_bullets[i].vy;
    }
}

void bullets_cull_offscreen(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        Bull *b = &g_bullets[i];
        if (!b->active) continue;
        if (b->x < -b->radius || b->x > SCREEN_W + b->radius ||
            b->y < -b->radius || b->y > SCREEN_H + b->radius) {
            b->active = false;
        }
    }
}

static bool circle_overlap(float ax, float ay, float ar, float bx, float by, float br) {
    float dx = ax - bx;
    float dy = ay - by;
    float rr = ar + br;
    return (dx * dx + dy * dy) <= (rr * rr);
}

void bullets_collide_character(Character *c, bool want_enemy_flag) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        Bull *b = &g_bullets[i];
        if (!b->active) continue;
        if (b->enemy != want_enemy_flag) continue;

        if (circle_overlap(c->x, c->y, c->radius, b->x, b->y, b->radius)) {
            b->active = false;
            c->lives--;
        }
    }
}

int bullet_wall_flags(float x, float y, float radius) {
    int result = 0;
    if (y - radius <= 0)          result |= WALL_UP;
    if (y + radius >= SCREEN_H)   result |= WALL_DOWN;
    if (x - radius <= 0)          result |= WALL_LEFT;
    if (x + radius >= SCREEN_W)   result |= WALL_RIGHT;
    return result;
}

/* Cheap filled-circle draw via per-pixel points, tinted by the current
 * render draw color. Fine for a handful of distinct sprites (player,
 * enemy) but too slow for hundreds of bullets -- see draw_filled_circle
 * note below. Swap to a pre-rendered texture + SDL_RenderCopy once you
 * have real bullet counts on screen. */
static void draw_filled_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, cx + x, cy + y);
            }
        }
    }
}

void bullets_draw(SDL_Renderer *renderer, SDL_Texture *circle_tex) {
    (void)circle_tex; /* unused until the texture-based draw path below is enabled */

    for (int i = 0; i < MAX_BULLETS; i++) {
        Bull *b = &g_bullets[i];
        if (!b->active) continue;

        if (b->enemy) {
            SDL_SetRenderDrawColor(renderer, 255, 60, 60, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 220, 80, 255);
        }
        draw_filled_circle(renderer, (int)b->x, (int)b->y, (int)b->radius);
    }
}
