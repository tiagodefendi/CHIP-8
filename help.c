#include "help.h"

void help() {
    printf("Como Usar o Emulador Chip-8\n");
    printf("A sintaxe básica para executar o programa é:\n");
    printf("```sh\n");
    printf("./chip8_emulator [opções] caminho/para/a/rom.ch8\n");
    printf("```\n");
    printf("## Opções de Linha de Comando\n");
    printf("`--clock <velocidade>`: Define a velocidade do clock da CPU em Hz. Padrão: 500Hz.\n");
    printf("**Exemplo:** `./chip8_emulator --clock 700 roms/tetris.ch8`\n");
    printf("`--scale <fator>`: Define o fator de escala da janela.\n");
    printf("Um fator de 10 resulta em uma janela de 640x320. Padrão: 10.\n");
    printf("**Exemplo:** `./chip8_emulator --scale 15 roms/pong.ch8`\n");
    printf("`--help`: Exibe esta mensagem de ajuda.\n");
}
