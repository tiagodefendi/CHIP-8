#include "c8vm.h"
#include "defs.h"
#include <SDL2/SDL.h>
#include "draw_display.h"
#include "audio.h"

// TODO: A velocidade de execução da CPU deve ser configurável via linha de comando (ex: 500Hz, 1000Hz).
// Consideramos que cada ciclo de CPU corresponde a uma instrução executada.
// Portanto, uma CPU rodando a 500Hz executa 500 instruções por segundo.

// TODO: A tela deve ser atualizada a uma taxa de 60Hz, independentemente da velocidade da CPU

// TODO: Os temporizadores (Delay Timer e Sound Timer) devem decrementar a uma taxa de 60Hz.

int main(int argc, char *argv[]) {
    // Configurações iniciais
    int cpu_hz = CPU_HZ; // Frequência da CPU em Hz
    int cycle_delay = CYCLE_DELAY; // Delay entre ciclos em milissegundos
    // caso o args 2 seja fornecido, sobrescreve cpu_hz
    if (argc >= 3) {
        cpu_hz = atoi(argv[2]);
        cycle_delay = 1000 / cpu_hz;
    }

    int scale = SCALE; // Fator de escala para o display
    // caso o args 3 seja fornecido, sobrescreve scale
    if (argc >= 4) {
        scale = atoi(argv[3]);
    }

    int prog_start = PROG_START; // Endereço de início do programa
    // caso o args 4 seja fornecido, sobrescreve prog_start
    if (argc >= 5) {
        prog_start = (int)strtol(argv[4], NULL, 16);
    }

    // Iniciar VM
    VM vm;
    VM_Inicializar(&vm, prog_start);
    int resultado = VM_CarregarROM(&vm, argv[1], prog_start);
    // Adicionar verificação de erro ao carregar a ROM
    if (!resultado) {
        printf("Falha ao carregar a ROM: %s\n", argv[1]);
        return -1;
    }
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
        // receber o tempo de inicio para cpu
        int cpu_start = SDL_GetTicks();

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
        int current_cpu_time = SDL_GetTicks();
        if (cycle_delay > SDL_GetTicks() - cpu_start) {
            SDL_Delay(cycle_delay - (SDL_GetTicks() - current_cpu_time));
        }
        VM_ExecutarInstrucao(&vm);
        #ifdef DEBUG
        VM_ImprimirRegistradores(&vm);
        #endif

        // render ==========================================================================

        draw_display(&vm, renderer, scale);

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
