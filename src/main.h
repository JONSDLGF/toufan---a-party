#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define SCREEN_W 800
#define SCREEN_H 600
#define FPS 60
#define FRAME_MS (1000 / FPS)

typedef enum {
    CF_MENU,
    CF_GAMEOVER,
    CF_WIN,
    CF_RUNGAME
} SceneType;

/* Keyboard input bitmask. Kept separate from wall-collision flags
 * (see bullet.h WALL_*) to avoid the naming collision the old
 * prototype had between "key held" and "touching wall". */
typedef enum {
    INPUT_NONE  = 0,
    INPUT_UP    = 1 << 0,
    INPUT_DOWN  = 1 << 1,
    INPUT_LEFT  = 1 << 2,
    INPUT_RIGHT = 1 << 3,
    INPUT_SHOOT = 1 << 4,
    INPUT_FOCUS = 1 << 5  /* slow-move / focus mode, common in danmaku */
} InputFlags;

typedef struct {
    int keys;       /* current frame */
    int keys_prev;  /* previous frame, for edge-detecting a press */
} Input;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font;	
    SDL_Texture  *circle_tex; /* pre-rendered white circle, tinted per-draw */
    
    bool          loop;
    SceneType     scene;
    SceneType     prev_scene;
    Input         input;
    Uint32        frame; /* global frame counter, useful for pattern timing */
} Engine;

static inline bool input_held(const Input *in, InputFlags f) {
    return (in->keys & f) != 0;
}

static inline bool input_pressed(const Input *in, InputFlags f) {
    return (in->keys & f) && !(in->keys_prev & f);
}

#endif
