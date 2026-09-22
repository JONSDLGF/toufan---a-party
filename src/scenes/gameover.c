#include "gameover.h"
#include <stdio.h>

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

    SDL_Rect box = {
        SCREEN_W / 2 - 180,
        SCREEN_H / 2 - 80,
        360,
        160
    };

    SDL_RenderFillRect(e->renderer, &box);

    SDL_Color white = {
        255, 255, 255, 255
    };

    SDL_Surface *surface = TTF_RenderUTF8_Blended(
        e->font,
        "GAME OVER",
        white
    );

    if (surface == NULL) {
        fprintf(stderr, "Error creando texto: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(
        e->renderer,
        surface
    );

    if (texture == NULL) {
        fprintf(stderr, "Error creando textura: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect text_rect = {
        SCREEN_W / 2 - surface->w / 2,
        SCREEN_H / 2 - surface->h / 2,
        surface->w,
        surface->h
    };

    SDL_RenderCopy(
        e->renderer,
        texture,
        NULL,
        &text_rect
    );

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
