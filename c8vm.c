#include "c8vm.h"
#include <stdio.h>
#include <stdlib.h>

void VM_Inicializar(VM* vm, uint16_t pc_inicial) {
    vm->PC = pc_inicial;
}

void VM_CarregarROM(VM* vm,
                    char* arq_rom,
                    uint16_t pc_inicial) {

    FILE* rom = fopen(arq_rom, "rb");
    fseek(rom, 0, SEEK_END);
    long tam_rom = ftell(rom);
    rewind(rom);

    fread(&vm->RAM[pc_inicial], 1, tam_rom, rom);

    fclose(rom);
}

void VM_ExecutarInstrucao(VM* vm) {
    uint16_t inst = (vm->RAM[vm->PC] << 8)
                    |vm->RAM[vm->PC + 1];

    printf("Instrução: 0x%04X\n", inst);
    vm->PC += 2;

    uint8_t grupo = inst >> 12;           // Grupo (4 bits mais significativos)
    uint8_t X     = (inst & 0x0F00) >> 8; // Registrador X (4 bits)
    uint8_t Y     = (inst & 0x00F0) >> 4; // Registrador Y (4 bits)
    uint8_t N     = inst & 0x000F;        // Nibble (4 bits)
    uint8_t NN    = inst & 0x00FF;        // Valor de 8 bits
    uint8_t NNN   = inst & 0x0FFF;        // Endereço de 12 bits

    switch(grupo){
        case 0x0:
            // CLS
            // Clear the display.
            if (inst == 0x00E0){
                for(int i = 0; i < 64*32; i++){
                    vm->DISPLAY[i] = 0;
                }
                break;
            }
            // RET
            // Return from a subroutine.
            // The interpreter sets the program counter to the address at the top of the stack,
            // then subtracts 1 from the stack pointer.
            if (inst == 0x00EE) {
                vm->PC = vm->stack[vm->SP];
                vm->SP--;
                break;
            }

            // SYS addr
            // Jump to a machine code routine at nnn.
            // This instruction is only used on the old computers on which Chip-8 was originally implemented.
            // It is ignored by modern interpreters.
            break;


        case 0x1:
            // JP addr
            // Jump to location nnn.
            // The interpreter sets the program counter to nnn.
            vm->PC = NNN;
            break;


        case 0x2:
            // CALL addr
            // Call subroutine at nnn.
            // The interpreter increments the stack pointer, then puts the current PC on the top of the stack.
            // The PC is then set to nnn.
            vm->SP++;
            vm->stack[vm->SP] = vm->PC;
            vm->PC = NNN;
            break;


        case 0x3:
            // SE Vx, byte
            // Skip next instruction if Vx = kk.
            // The interpreter compares register Vx to kk, and if they are equal,
            // increments the program counter by 2.
            if (vm->V[X] == NN) {
                vm->PC += 2;
            }
            break;


        case 0x4:
            // SNE Vx, byte
            // Skip next instruction if Vx != kk.
            // The interpreter compares register Vx to kk, and if they are not equal,
            // increments the program counter by 2.
            if (vm->V[X] != NN) {
                vm->PC += 2;
            }
            break;

        case 0x5:
            // SE Vx, Vy
            // Skip next instruction if Vx = Vy.
            // The interpreter compares register Vx to register Vy, and if they are equal,
            // increments the program counter by 2.
            if (vm->V[X] == vm->V[Y]) {
                vm->PC += 2;
            }
            break;

        case 0x6:
            // LD Vx, byte
            // Set Vx = kk.
            // The interpreter puts the value kk into register Vx.
            vm->V[X] = NN;
            break;

        case 0x7:
            // ADD Vx, byte
            // Set Vx = Vx + kk.
            // Adds the value kk to the value of register Vx, then stores the result in Vx.
            vm->V[X] += NN;
            break;

        case 0x8:
            // N Representa os 4 últimos bits menos signficativos

            // LD Vx, Vy
            // Set Vx = Vy.
            // Stores the value of register Vy in register Vx.
            if (N == 0) {
                vm->V[X] = vm->V[Y];
                break;
            }

            // OR Vx, Vy
            // AND Vx, Vy
            // XOR Vx, Vy
            // ADD Vx, Vy
            // SUB Vx, Vy
            // SHR Vx {, Vy}
            // SUBN Vx, Vy
            // SHL Vx {, Vy}
            break;

        case 0x9:
            // SNE Vx, Vy
            break;

        case 0xA:
            // LD I, addr
            break;

        case 0xB:
            // JP V0, addr
            break;

        case 0xC:
            // RND Vx, byte
            break;


        // TODO: DESENHAR NA TELA
        case 0xD:
            // DRW Vx, Vy, nibble
            break;

        case 0xE:
            // SKP Vx
            // SKNP Vx
            break;

        case 0xF:
            // LD Vx, DT
            // LD Vx, K
            // LD DT, Vx
            // LD ST, Vx
            // ADD I, Vx
            // LD F, Vx
            // LD B, Vx
            // LD [I], Vx
            // LD Vx, [I]
            break;

        default:
            printf("Grupo não implementado! Instrução: 0x%04X\n", inst);
            exit(1); // stdlib
    }
}

void VM_ImprimirRegistradores(VM* vm) {
    printf("PC: 0x%04X I: 0x%04X SP: 0x%02X\n", vm->PC, vm->I, vm->SP);
    for(int i = 0; i < 16; i++) {
        printf("V[%X]: 0x%02X ", i, vm->V[i]);
    }
    printf("\n");
}
