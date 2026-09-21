#include "menu.h"

void scene_menu_enter(Engine *e) {
    (void)e;
}

void update_scene_menu(Engine *e) {
    if (input_pressed(&e->input, INPUT_SHOOT)) {
        e->scene = CF_RUNGAME;
    }
}

void draw_scene_menu(Engine *e) {
    /* TODO: draw title text via SDL_ttf. Left blank on purpose --
       plug in your font rendering here. */
    SDL_SetRenderDrawColor(e->renderer, 40, 40, 60, 255);
    SDL_Rect box = { SCREEN_W / 2 - 100, SCREEN_H / 2 - 20, 200, 40 };
    SDL_RenderFillRect(e->renderer, &box);
}
