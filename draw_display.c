#include "draw_display.h"

/**
 * @brief Desenha o display da VM na janela SDL.
 * @param vm Ponteiro para a estrutura da VM.
 * @param renderer Ponteiro para o renderizador SDL.
 * @param scale Fator de escala para o display.
 */
void draw_display(VM* vm, SDL_Renderer* renderer, int scale) {
    // Limpa Tela
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Preto
    SDL_RenderClear(renderer);

    // Desenha Pixels
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Branco
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            if (vm->DISPLAY[y * DISPLAY_WIDTH + x]) {
                // Gera o pixel com escala
                SDL_Rect pixel = {
                    x * scale,
                    y * scale,
                    scale,
                    scale
                };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    // Atualiza a Tela
    SDL_RenderPresent(renderer);
}
