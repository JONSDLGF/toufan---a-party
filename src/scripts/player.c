#include "player.h"

void player_init(Character *player) {
    player->x = SCREEN_W / 2.0f;
    player->y = SCREEN_H * 0.8f;
    player->radius = 4.0f;   /* hitbox: small, danmaku-style */
    player->lives = 3;
    player->shot_timer = 0;
}

void player_update(Character *player, const Input *input) {
    float speed = input_held(input, INPUT_FOCUS) ? PLAYER_SPEED_FOCUS : PLAYER_SPEED;

    /* Check against the sprite-ish radius so the player can't visually
     * clip through a wall; using the small hitbox radius here would
     * let the sprite overlap the screen edge. */
    float visual_radius = 14.0f;
    int wall = bullet_wall_flags(player->x, player->y, visual_radius);

    if (input_held(input, INPUT_UP)    && !(wall & WALL_UP))    player->y -= speed;
    if (input_held(input, INPUT_DOWN)  && !(wall & WALL_DOWN))  player->y += speed;
    if (input_held(input, INPUT_LEFT)  && !(wall & WALL_LEFT))  player->x -= speed;
    if (input_held(input, INPUT_RIGHT) && !(wall & WALL_RIGHT)) player->x += speed;

    if (player->shot_timer > 0) player->shot_timer--;

    if (input_held(input, INPUT_SHOOT) && player->shot_timer == 0) {
        bullet_spawn(player->x, player->y - visual_radius, 0.0f, -9.0f, 4.0f, false);
        player->shot_timer = PLAYER_SHOT_COOLDOWN;
    }
}

void player_draw(const Character *player, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int r = 14;
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x * x + y * y <= r * r) {
                SDL_RenderDrawPoint(renderer, (int)player->x + x, (int)player->y + y);
            }
        }
    }
}
