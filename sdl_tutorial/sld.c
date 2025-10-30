#include <SDL2/SDL.h>

#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS)

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    int speed = 5;

    int posx = WINDOW_WIDTH / 2;
    int posy = WINDOW_HEIGHT / 2;

    int box_w = 40;
    int box_h = 40;

    SDL_Event e;
    int quit = 0;
    // loop principal
    while (!quit) {
        // receber o tempo de inicio do frame
        int frame_start = SDL_GetTicks();

        // input de eventos =================================================================
        while (SDL_PollEvent(&e) != 0) {
            if (
                e.type == SDL_QUIT ||
                (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            ) {
                quit = 1;
            }
        }

        // update ==========================================================================

        // fazer caixa se mover com as setas
        const Uint8* keyboard = SDL_GetKeyboardState(NULL);
        if (keyboard[SDL_SCANCODE_UP]) {
            posy -= speed;
        }
        if (keyboard[SDL_SCANCODE_DOWN]) {
            posy += speed;
        }
        if (keyboard[SDL_SCANCODE_LEFT]) {
            posx -= speed;
        }
        if (keyboard[SDL_SCANCODE_RIGHT]) {
            posx += speed;
        }
        // caso a caixa saia da tela, resetar posicao
        if (posx < 0) {
            posx = 0;
        }
        if (posx > WINDOW_WIDTH - box_w) {
            posx = WINDOW_WIDTH - box_w;
        }
        if (posy < 0) {
            posy = 0;
        }
        if (posy > WINDOW_HEIGHT - box_h) {
            posy = WINDOW_HEIGHT - box_h;
        }

        // render ==========================================================================
        // clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw screen
        SDL_Rect box = {posx, posy, box_w, box_h};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &box);
        SDL_RenderPresent(renderer);

        // controlar o fps =================================================================
        int current_frame = SDL_GetTicks();
        if (FRAME_DELAY > SDL_GetTicks() - frame_start) {
            SDL_Delay(FRAME_DELAY - (SDL_GetTicks() - current_frame));
        }
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}
