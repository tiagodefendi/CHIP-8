#include "c8vm.h"
#include "defs.h"
#include <SDL2/SDL.h>
#include "draw_display.h"
#include "audio.h"
#include "help.h"

// TODO: A tela deve ser atualizada a uma taxa de 60Hz, independentemente da velocidade da CPU

// TODO: Os temporizadores (Delay Timer e Sound Timer) devem decrementar a uma taxa de 60Hz.

int main(int argc, char *argv[]) {
    // Configurações iniciais
    int cpu_hz = CPU_HZ; // Frequência da CPU em Hz
    int scale = SCALE; // Fator de escala da janela
    int prog_start = PROG_START; // Endereço inicial do programa na memória
    const char *rom_path = NULL; // Caminho para o arquivo ROM

    // Parse manual dos argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--clock") == 0 && i + 1 < argc) {
            cpu_hz = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--scale") == 0 && i + 1 < argc) {
            scale = atoi(argv[++i]);
        } else if (argv[i][0] != '-') {
            // primeiro argumento sem "-" é o caminho da ROM
            rom_path = argv[i];
        } else {
            printf("Opcao desconhecida: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    if (!rom_path) {
        printf("Erro: Nenhum arquivo ROM fornecido.\n");
        print_help(argv[0]);
        return 1;
    }

    int cycle_delay = 1000 / cpu_hz;

    // Iniciar VM
    VM vm;
    VM_Inicializar(&vm, prog_start);
    int resultado = VM_CarregarROM(&vm, rom_path, prog_start);
    // Adicionar verificação de erro ao carregar a ROM
    if (!resultado) {
        printf("Falha ao carregar a ROM: %s\n", rom_path);
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
