#include "gameover.h"

void scene_gameover_enter(Engine *e) {
    (void)e;
}

void update_scene_gameover(Engine *e) {
    if (input_pressed(&e->input, INPUT_SHOOT)) {
        e->scene = CF_MENU;
    }
}

void draw_scene_gameover(Engine *e) {
    SDL_SetRenderDrawColor(e->renderer, 60, 20, 20, 255);
    SDL_Rect box = { SCREEN_W / 2 - 120, SCREEN_H / 2 - 20, 240, 40 };
    SDL_RenderFillRect(e->renderer, &box);
}
