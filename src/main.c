#include "main.h"
#include "scenes/menu.h"
#include "scenes/gameover.h"
#include "scenes/win.h"
#include "scenes/game.h"
#include <stdio.h>

typedef void (*SceneEnterFn)(Engine *);
typedef void (*SceneFn)(Engine *);

static const SceneEnterFn enter_fns[] = {
    [CF_MENU]     = scene_menu_enter,
    [CF_GAMEOVER] = scene_gameover_enter,
    [CF_WIN]      = scene_win_enter,
    [CF_RUNGAME]  = scene_game_enter,
};

static const SceneFn update_fns[] = {
    [CF_MENU]     = update_scene_menu,
    [CF_GAMEOVER] = update_scene_gameover,
    [CF_WIN]      = update_scene_win,
    [CF_RUNGAME]  = update_scene_game,
};

static const SceneFn draw_fns[] = {
    [CF_MENU]     = draw_scene_menu,
    [CF_GAMEOVER] = draw_scene_gameover,
    [CF_WIN]      = draw_scene_win,
    [CF_RUNGAME]  = draw_scene_game,
};

/* Maps a raw SDLK_* code to a bit in InputFlags, or INPUT_NONE if it's
 * not one we care about. Centralizing this means SDL_KEYDOWN and
 * SDL_KEYUP handling below can't drift out of sync with each other. */
static InputFlags map_key(SDL_Keycode sym) {
    switch (sym) {
        case SDLK_UP:    return INPUT_UP;
        case SDLK_DOWN:  return INPUT_DOWN;
        case SDLK_LEFT:  return INPUT_LEFT;
        case SDLK_RIGHT: return INPUT_RIGHT;
        case SDLK_SPACE: return INPUT_SHOOT;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT: return INPUT_FOCUS;
        default: return INPUT_NONE;
    }
}

static void handle_events(Engine *e) {
    e->input.keys_prev = e->input.keys;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                e->loop = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    e->loop = false;
                    break;
                }
                e->input.keys |= map_key(event.key.keysym.sym);
                break;

            case SDL_KEYUP:
                e->input.keys &= ~map_key(event.key.keysym.sym);
                break;

            default:
                break;
        }
    }
}

static void dispatch_scene_transition(Engine *e) {
    if (e->scene != e->prev_scene) {
        enter_fns[e->scene](e);
        e->prev_scene = e->scene;
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    Engine engine = {0};
    engine.loop = true;
    engine.scene = CF_MENU;
    engine.prev_scene = CF_MENU; /* set to same as scene so enter_fns
                                    below still fires once via the
                                    forced call before the loop */

    engine.window = SDL_CreateWindow(
        "Danmaku",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN
    );
    if (!engine.window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    engine.renderer = SDL_CreateRenderer(
        engine.window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!engine.renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(engine.window);
        SDL_Quit();
        return 1;
    }

    engine.font = TTF_OpenFont(
        "assets/fonts/dejavu/DejaVuSans-Bold.ttf",
        42
    );

    if (engine.font == NULL) {
        fprintf(stderr, "TTF_OpenFont failed: %s\n", TTF_GetError());

        SDL_DestroyRenderer(engine.renderer);
        SDL_DestroyWindow(engine.window);
        TTF_Quit();
        SDL_Quit();

        return 1;
    }

    engine.circle_tex = NULL; /* TODO: pre-render a circle texture here
                                  and swap bullets_draw's per-pixel loop
                                  for SDL_RenderCopy once bullet counts
                                  get large -- see bullet.c note. */

    /* Force the initial scene's enter hook to run once before the
     * loop starts, since prev_scene == scene right now. */
    enter_fns[engine.scene](&engine);

    Uint32 last_tick = SDL_GetTicks();

    while (engine.loop) {
        handle_events(&engine);
        dispatch_scene_transition(&engine);

        Uint32 now = SDL_GetTicks();
        while (now - last_tick >= FRAME_MS) {
            update_fns[engine.scene](&engine);
            dispatch_scene_transition(&engine); /* scene may have just changed */
            engine.frame++;
            last_tick += FRAME_MS;
        }

        SDL_SetRenderDrawColor(engine.renderer, 20, 20, 30, 255);
        SDL_RenderClear(engine.renderer);

        draw_fns[engine.scene](&engine);

        SDL_RenderPresent(engine.renderer);
    }

    if (engine.circle_tex) SDL_DestroyTexture(engine.circle_tex);
    SDL_DestroyRenderer(engine.renderer);
    SDL_DestroyWindow(engine.window);
    SDL_Quit();

    return 0;
}
