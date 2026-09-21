#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "../main.h"

/* Called once when the engine transitions INTO CF_RUNGAME (see the
 * enter/exit dispatch in main.c). Resets bullet pool, player, enemy --
 * this is the piece the original single-file prototype was missing,
 * so leftover bullets/state didn't leak between playthroughs. */
void scene_game_enter(Engine *e);

void update_scene_game(Engine *e);
void draw_scene_game(Engine *e);

#endif
