#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

int key;
int sw = 800, sh = 600;

#define MAX_BULLETS 100 // ok funciona el maxio de bolas XD "bolas"

typedef struct {
    bool loop;
} Engine;

typedef struct {
    int x, y;
    int vx, vy;
    int radio;
    bool enemy;
    bool active;
} Bull;

typedef struct {
    int x, y;
    int lives;
    int radio;
    int timer;
} Character;

bool make_Bull(int x, int y, int vx, int vy, int radio, bool enemy, Bull list_bull[]){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(!list_bull[i].active){
            list_bull[i].x=x;
            list_bull[i].y=y;
            list_bull[i].vx=vx;
            list_bull[i].vy=vy;
            list_bull[i].radio=radio;
            list_bull[i].enemy=enemy;
            list_bull[i].active=true;
            return true;
        }
    }
    return false;
}

bool collision_Bull(Character player, Bull bullet) {
    int dx = player.x - bullet.x;
    int dy = player.y - bullet.y;

    int distancia_cuadrada = dx * dx + dy * dy;
    int radios = player.radio + bullet.radio;

    return distancia_cuadrada <= radios * radios;
}

#define COLLISION_UP      0x01
#define COLLISION_DOWN    0x02
#define COLLISION_LEFT    0x04
#define COLLISION_RIGHT   0x08
#define K_bull            0x10
#define TIME_SLEEP_BULL_P 20

int collision(int x, int y, int radio) {
    int result = 0;

    if (y - radio <= 0) {
        result |= COLLISION_UP;
    }

    if (y + radio >= sh) {
        result |= COLLISION_DOWN;
    }

    if (x - radio <= 0) {
        result |= COLLISION_LEFT;
    }

    if (x + radio >= sw) {
        result |= COLLISION_RIGHT;
    }

    return result;
}

void mov_bulls(Bull list_bull[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!list_bull[i].active) {
            continue;
        }

        list_bull[i].x += list_bull[i].vx;
        list_bull[i].y += list_bull[i].vy;
    }
}

void colision_ball_player(Character *player, Bull list_bull[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!list_bull[i].active) {
            continue;
        }

        if (!list_bull[i].enemy) {
            continue;
        }

        if (collision_Bull(*player, list_bull[i])) {
            list_bull[i].active = false;

            if (list_bull[i].enemy) {
                player->lives--;
            }
        }
    }
}

void colision_ball_enemy(Character *enemy, Bull list_bull[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!list_bull[i].active) {
            continue;
        }
        if (list_bull[i].enemy){
            continue;
        }

        if (collision_Bull(*enemy, list_bull[i])) {
            list_bull[i].active = false;

            if (!list_bull[i].enemy) {
                enemy->lives--;
            }
        }
    }
}

void player_acts(Character *player, Bull list_bull[]){
    int col = collision(player->x, player->y, player->radio);

    if ((key & COLLISION_UP) && !(col & COLLISION_UP)) {
        player->y -= 5;
    }

    if ((key & COLLISION_DOWN) && !(col & COLLISION_DOWN)) {
        player->y += 5;
    }

    if ((key & COLLISION_LEFT) && !(col & COLLISION_LEFT)) {
        player->x -= 5;
    }

    if ((key & COLLISION_RIGHT) && !(col & COLLISION_RIGHT)) {
        player->x += 5;
    }
    
    if (player->timer > 0) {
        player->timer--;
    }

    if ((key & K_bull) && player->timer == 0) {
        make_Bull(
            player->x,
            player->y - player->radio,
            0,
            -6,
            10,
            false,
            list_bull
        );

        player->timer = TIME_SLEEP_BULL_P;
    }
}

void destroy_bullets(Bull list_bull[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!list_bull[i].active) {
            continue;
        }

        if (list_bull[i].x < -list_bull[i].radio ||
            list_bull[i].x > sw + list_bull[i].radio ||
            list_bull[i].y < -list_bull[i].radio ||
            list_bull[i].y > sh + list_bull[i].radio) {
            list_bull[i].active = false;
        }
    }
}

void enemy_ai(Character *enemy, Bull list_bull[]){
    if(enemy->timer==0){
        make_Bull(enemy->x, enemy->y, 1, 1, 30, true, list_bull);
        enemy->timer=100;
        return;
    }
    enemy->timer-=1;
}

void update(Character *player, Character *enemy, Bull list_bull[]) {
    mov_bulls(list_bull);
    colision_ball_player(player, list_bull);
    colision_ball_enemy(enemy, list_bull);
    destroy_bullets(list_bull);
    player_acts(player, list_bull);
    enemy_ai(enemy, list_bull);
}

void draw_filled_circle(SDL_Renderer *renderer, int center_x, int center_y, int radius){
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(
                    renderer,
                    center_x + x,
                    center_y + y
                );
            }
        }
    }
}

Bull list_bull[MAX_BULLETS];
Character player;
Character enemy;

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    player.radio = 25;
    player.x = sw / 2;
    player.y = sh / 2;
    player.lives = 3;
    
    enemy.radio = 30;
    enemy.x = sw / 2;
    enemy.y = (sh / 2) - 80;
    enemy.lives = 2;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Main Loop SDL2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        sw, sh,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        fprintf(stderr, "Error al crear la ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL) {
        fprintf(stderr, "Error al crear el renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Engine engine;
    engine.loop=true;

    SDL_Event event;

    while (engine.loop) {
        /*
         * 1. Procesar eventos
         */
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    engine.loop=false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            key |= COLLISION_UP;
                            break;

                        case SDLK_DOWN:
                            key |= COLLISION_DOWN;
                            break;

                        case SDLK_LEFT:
                            key |= COLLISION_LEFT;
                            break;

                        case SDLK_RIGHT:
                            key |= COLLISION_RIGHT;
                            break;

                        case SDLK_SPACE:
                            key |= K_bull;
                            break;

                        case SDLK_ESCAPE:
                            engine.loop = false;
                            break;

                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            key &= ~COLLISION_UP;
                            break;

                        case SDLK_DOWN:
                            key &= ~COLLISION_DOWN;
                            break;

                        case SDLK_LEFT:
                            key &= ~COLLISION_LEFT;
                            break;

                        case SDLK_RIGHT:
                            key &= ~COLLISION_RIGHT;
                            break;

                        case SDLK_SPACE:
                            key &= ~K_bull;
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        /*
         * 2. Actualizar la lógica del juego
         */
        update( &player, &enemy, list_bull );

        /*
         * 3. Limpiar la pantalla
         */
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        /*
         * 4. Dibujar
         */

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        draw_filled_circle(renderer, player.x, player.y, player.radio);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        draw_filled_circle(renderer, enemy.x, enemy.y, enemy.radio);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for ( int i=0; i < MAX_BULLETS; i++ ) {
            if (!list_bull[i].active) {
                continue;
            }
            draw_filled_circle(renderer, list_bull[i].x, list_bull[i].y, list_bull[i].radio);
        }

        /*
         * 5. Mostrar el frame
         */
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
