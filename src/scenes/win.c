#include "win.h"

void scene_win_enter(Engine *e) {
    (void)e;
}

void update_scene_win(Engine *e) {
    if (input_pressed(&e->input, INPUT_SHOOT)) {
        e->scene = CF_MENU;
    }
}

void draw_scene_win(Engine *e) {
    SDL_SetRenderDrawColor(e->renderer, 20, 60, 20, 255);
    SDL_Rect box = { SCREEN_W / 2 - 80, SCREEN_H / 2 - 20, 160, 40 };
    SDL_RenderFillRect(e->renderer, &box);
}
