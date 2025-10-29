#include "c8vm.h"
#include "defs.h"
#include <SDL2/SDL.h>
#include "draw_display.h"
#include "audio.h"

int main(int argc, char *argv[]) {
    // Configurações iniciais
    int scale = 10; // Fator de escala para o display

    // Iniciar VM
    VM vm;
    VM_Inicializar(&vm, 0x200);
    VM_CarregarROM(&vm, argv[1], 0x200);
    #ifdef DEBUG
    VM_ImprimirRegistradores(&vm);
    #endif

    // Inicializar SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * scale, DISPLAY_HEIGHT * scale,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec want = {0};
    want.freq = 44100;
    want.format = AUDIO_U8;
    want.channels = 1;
    want.samples = 2048;
    audio_device = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    SDL_PauseAudioDevice(audio_device, 0); // habilita audio
    uint8_t sound_buffer[4410]; // 0,1 segundo de som
    for (int i = 0; i < 4410; i++) {
        sound_buffer[i] = (i / 50) % 2 ? 255 : 0;
    }

    // Iniciando os Eventos e o Loop principal
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
        VM_ExecutarInstrucao(&vm);
        #ifdef DEBUG
        VM_ImprimirRegistradores(&vm);
        #endif

        // render ==========================================================================

        if (vm.draw_flag) {
            draw_display(&vm, renderer, scale);
            vm.draw_flag = 0;
        }

        // controlar o fps =================================================================
        int current_frame = SDL_GetTicks();
        if (FRAME_DELAY > SDL_GetTicks() - frame_start) {
            SDL_Delay(FRAME_DELAY - (SDL_GetTicks() - current_frame));

            // delay timer
            if (vm.delay_timer > 0) {
                vm.delay_timer--;
            }

            // sound timer
            if (vm.sound_timer > 0) {
                if (SDL_GetQueuedAudioSize(audio_device) < sizeof(sound_buffer)) {
                    play_sound(audio_device, sound_buffer);
                }
                vm.sound_timer--;
            } else {
                stop_sound(audio_device);
            }
        }
    }

    return 0;
}
