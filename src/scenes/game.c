#include "game.h"
#include "../scripts/player.h"
#include "../scripts/enemy.h"
#include "../scripts/bullet.h"

/* Scene-local state. Kept file-static rather than global so nothing
 * outside this scene can poke at it directly -- scripts only see the
 * Character pointers/values they're handed. */
static Character player;
static Character enemy;

void scene_game_enter(Engine *e) {
    (void)e;
    bullet_pool_reset();
    player_init(&player);
    enemy_init(&enemy);
}

void update_scene_game(Engine *e) {
    player_update(&player, &e->input);
    enemy_update(&enemy, player.x, player.y);

    bullets_update();
    bullets_cull_offscreen();

    /* Player bullets (enemy == false) hurt the enemy;
       enemy bullets (enemy == true) hurt the player. */
    bullets_collide_character(&enemy, false);
    bullets_collide_character(&player, true);

    if (player.lives <= 0) {
        e->scene = CF_GAMEOVER;
    } else if (enemy.lives <= 0) {
        e->scene = CF_WIN;
    }
}

void draw_scene_game(Engine *e) {
    player_draw(&player, e->renderer);
    enemy_draw(&enemy, e->renderer);
    bullets_draw(e->renderer, e->circle_tex);
}
