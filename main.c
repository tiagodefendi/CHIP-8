#include "c8vm.h"
#include "defs.h"
#include <SDL2/SDL.h>
#include "draw_display.h"
#include "audio.h"
#include "help.h"

int main(int argc, char *argv[]) {
    // Configurações iniciais
    int frame_hz = TARGET_FPS; // Frames por segundo
    int cpu_hz = CPU_HZ; // Frequência da CPU em Hz
    int scale = SCALE; // Fator de escala da janela
    int prog_start = PROG_START; // Endereço inicial do programa na memória
    char *rom_path = NULL; // Caminho para o arquivo ROM

    // Parse manual dos argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            help();
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
            help();
            return 1;
        }
    }

    if (!rom_path) {
        printf("Erro: Nenhum arquivo ROM fornecido.\n");
        help();
        return 1;
    }

    // Calcula a duração dos frames e ciclos da CPU em milissegundos
    const double frame_duration = 1000.0 / frame_hz;
    const double cpu_cycle_duration = 1000.0 / cpu_hz;
    double frame_accumulator = 0.0;
    double cpu_accumulator = 0.0;
    uint32_t last_time = SDL_GetTicks();

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
    // Configurando o audio
    want.freq = 44100;          // taxa de amostragem padrao (44.1 kHz)
    want.format = AUDIO_U8;     // 8 bits sem sinal
    want.channels = 1;          // mono
    want.samples = 2048;        // tamanho do buffer
    audio_device = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    SDL_PauseAudioDevice(audio_device, 0); // habilita audio

    // Gerando o som em A4 (440 Hz)
    uint8_t sound_buffer[44100]; // 1 segundo de som
    int sample_rate = 44100;
    int freq = 440;
    int half_period = sample_rate / (freq * 2); // metade do ciclo da onda

    for (int i = 0; i < sample_rate; i++) {
        sound_buffer[i] = ((i / half_period) % 2) ? 255 : 0;
    }

    // Iniciando os Eventos e o Loop principal
    SDL_Event e;
    int quit = 0;
    // loop principal
    while (!quit) {
        // Calcula tempo gasto na execução do laço
        uint32_t current_time = SDL_GetTicks();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Incrementa os acumuladores
        frame_accumulator += delta_time;
        cpu_accumulator += delta_time;

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

        // Executa ciclos da CPU conforme o acumulador
        while (cpu_accumulator >= cpu_cycle_duration) {
            // Executa uma instrução da VM
            VM_ExecutarInstrucao(&vm);
            // Decrementa o acumulador de CPU
            cpu_accumulator -= cpu_cycle_duration;
        }

        #ifdef DEBUG
        VM_ImprimirRegistradores(&vm);
        #endif

        // render ==========================================================================

        draw_display(&vm, renderer, scale);

        // controlar o fps =================================================================
        if (frame_accumulator >= frame_duration) {

            // delay timer
            if (vm.delay_timer > 0) {
                vm.delay_timer--;
            }

            // sound timer
            if (vm.sound_timer > 0) {
                if (SDL_GetQueuedAudioSize(audio_device) < sizeof(sound_buffer)) {
                    play_sound(audio_device, sound_buffer, sizeof(sound_buffer));
                }
                vm.sound_timer--;
            } else {
                stop_sound(audio_device);
            }

            // Decrementa o acumulador de frame
            frame_accumulator -= frame_duration;
        }
    }

    // Encerramento limpo
    if (audio_device)
        SDL_CloseAudioDevice(audio_device);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
