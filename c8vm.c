#include "c8vm.h"
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <SDL2/SDL.h>
#include "keyboard_map.h"

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
            if (N == 0x0) {
                vm->V[X] = vm->V[Y];
                break;
            }

            // OR Vx, Vy
            // Set Vx = Vx OR Vy.
            // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
            // A bitwise OR compares the corrseponding bits from two values, and if either bit is 1,
            // then the same bit in the result is also 1. Otherwise, it is 0.
            if (N == 0x1) {
                vm->V[X] = vm->V[X] | vm->V[Y];
                break;
            }

            // AND Vx, Vy
            // Set Vx = Vx AND Vy.
            // Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
            // A bitwise AND compares the corrseponding bits from two values, and if both bits are 1,
            // then the same bit in the result is also 1. Otherwise, it is 0.
            if (N == 0x2) {
                vm->V[X] = vm->V[X] & vm->V[Y];
                break;
            }

            // XOR Vx, Vy
            // Set Vx = Vx XOR Vy.
            // Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
            // An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same,
            // then the corresponding bit in the result is set to 1. Otherwise, it is 0.
            if (N == 0x3) {
                vm->V[X] = vm->V[X] ^ vm->V[Y];
                break;
            }

            // ADD Vx, Vy
            // Set Vx = Vx + Vy, set VF = carry.
            // The values of Vx and Vy are added together.
            // If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
            // Only the lowest 8 bits of the result are kept, and stored in Vx.
            if (N == 0x4) {
                if (vm->V[X] + vm->V[Y] > 0xFF) {
                    vm->V[0xF] = 1;
                } else {
                    vm->V[0xF] = 0;
                }
                vm->V[X] = vm->V[X] + vm->V[Y];
                break;
            }

            // SUB Vx, Vy
            // Set Vx = Vx - Vy, set VF = NOT borrow.
            // If Vx > Vy, then VF is set to 1, otherwise 0.
            // Then Vy is subtracted from Vx, and the results stored in Vx.
            if (NN == 0x5) {
                if(vm->V[X] > vm->V[Y]) {
                    vm->V[0xF] = 1;
                } else {
                    vm->V[0xF] = 0;
                }
                vm->V[X] = vm->V[X] - vm->V[Y];
                break;
            }

            // SHR Vx {, Vy}
            // Set Vx = Vx SHR 1.
            // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
            if (NN == 0x6) {
                vm->V[0xF] = vm->V[X] & 0x01;
                vm->V[X] >>= 2;
                break;
            }

            // SUBN Vx, Vy
            // Set Vx = Vy - Vx, set VF = NOT borrow.
            // If Vy > Vx, then VF is set to 1, otherwise 0.
            // Then Vx is subtracted from Vy, and the results stored in Vx.
            if (NN == 0x7) {
                if(vm->V[Y] > vm->V[X]) {
                    vm->V[0xF] = 1;
                } else {
                    vm->V[0xF] = 0;
                }
                vm->V[X] = vm->V[Y] - vm->V[X];
                break;
            }

            // SHL Vx {, Vy}
            //Set Vx = Vx SHL 1.
            // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
            if (NN == 0xE) {
                vm->V[0xF] = vm->V[X] & 0x80;
                vm->V[X] <<= 1;
                break;
            }

            break;


        case 0x9:
            // SNE Vx, Vy
            // Skip next instruction if Vx != Vy.
            // The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
            if (vm->V[X] != vm->V[Y]) {
                vm->PC += 2;
            }
            break;


        case 0xA:
            // LD I, addr
            // Set I = nnn.
            // The value of register I is set to nnn.
            vm->I = NNN;
            break;


        case 0xB:
            // JP V0, addr
            // Jump to location nnn + V0.
            // The program counter is set to nnn plus the value of V0.
            vm->PC = NNN + vm->V[0x0];
            break;


        case 0xC:
            // RND Vx, byte
            // Set Vx = random byte AND kk.
            // The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk.
            // The results are stored in Vx. See instruction 8xy2 for more information on AND.
            vm->V[X] = (rand() % 0xFF) & NN;
            break;


        case 0xD:
            // DRW Vx, Vy, nibble
            // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
            // The interpreter reads n bytes from memory, starting at the address stored in I.
            // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
            // Sprites are XORed onto the existing screen.
            // If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
            // If the sprite is positioned so part of it is outside the coordinates of the display,
            // it wraps around to the opposite side of the screen.
            // See instruction 8xy3 for more information on XOR, and section 2.4, Display,
            // for more information on the Chip-8 screen and sprites.
            uint8_t xcoord = vm->V[X] % DISPLAY_WIDTH;
            uint8_t ycoord = vm->V[Y] % DISPLAY_HEIGHT;
            for (int row = 0; row < N; row++) {
                uint8_t bits = vm->RAM[vm->I + row];
                uint8_t cy = (ycoord + row) % DISPLAY_HEIGHT;

                for (int col = 0; col < 8; col++) {
                    uint8_t cx = (xcoord + col) % DISPLAY_WIDTH;
                    uint8_t curr_col = vm->DISPLAY[cy * DISPLAY_WIDTH + cx];
                    uint8_t bit = bits & (0x01 << (7 - col));

                    if (bit > 0) {
                        if (curr_col > 0) {
                            vm->DISPLAY[cy * DISPLAY_WIDTH + cx] = 0;
                            vm->V[0xF] = 1;
                        } else {
                            vm->DISPLAY[cy * DISPLAY_WIDTH + cx] = 1;
                        }
                    }

                    if (cx == DISPLAY_WIDTH - 1) {
                        break;
                    }
                }

                if (cy == DISPLAY_HEIGHT - 1) {
                    break;
                }
            }
            vm->draw_flag = 1;
            break;


        case 0xE:
            // NN Representa os 8 últimos bits menos signficativos

            // SKP Vx
            // Skip next instruction if key with the value of Vx is pressed.
            // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
            if (NN == 0x9E) {
                const Uint8* keyboard = SDL_GetKeyboardState(NULL);
                if (keyboard[map_reverse_scancodes(vm->V[X])]) {
                    vm->PC += 2;
                }
                break;
            }

            // SKNP Vx
            // Skip next instruction if key with the value of Vx is not pressed.
            // Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
            if (NN == 0xA1) {
                const Uint8* keyboard = SDL_GetKeyboardState(NULL);
                if (!keyboard[map_reverse_scancodes(vm->V[X])]) {
                    vm->PC += 2;
                }
                break;
            }
            break;


        case 0xF:
            // NN Representa os 8 últimos bits menos signficativos

            // LD Vx, DT
            // Set Vx = delay timer value.
            // The value of DT is placed into Vx.
            if (NN == 0x07) {
                vm->V[X] = vm->delay_timer;
                break;
            }

            // LD Vx, K
            // Wait for a key press, store the value of the key in Vx.
            // All execution stops until a key is pressed, then the value of that key is stored in Vx.
            if (NN == 0x0A) {
                SDL_Event e;
                int quit = 0;
                while (!quit) {
                    if (SDL_PollEvent(&e)) {
                        if (e.type == SDL_KEYDOWN) {
                            if (map_keyboard_keys(e.key.keysym.sym) != 0xFF) {
                                vm->V[X] = map_keyboard_keys(e.key.keysym.sym);
                                quit = 1;
                            }
                        }
                    }
                }
                break;
            }

            // LD DT, Vx
            // Set delay timer = Vx.
            // DT is set equal to the value of Vx.
            if (NN == 0x15) {
                vm->delay_timer = vm->V[X];
                break;
            }

            // LD ST, Vx
            // Set sound timer = Vx.
            // ST is set equal to the value of Vx.
            if (NN == 0x18) {
                vm->sound_timer = vm->V[X];
                break;
            }

            // ADD I, Vx
            // Set I = I + Vx.
            // The values of I and Vx are added, and the results are stored in I.
            if (NN == 0x1E) {
                vm->I += vm->V[X];
                break;
            }

            // LD F, Vx
            // Set I = location of sprite for digit Vx.
            // The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
            // See section 2.4, Display, for more information on the Chip-8 hexadecimal font.
            if (NN == 0x29) {
                vm->I = vm->V[X] * 0x05;
                break;
            }

            // LD B, Vx
            // Store BCD representation of Vx in memory locations I, I+1, and I+2.
            // The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I,
            // the tens digit at location I+1, and the ones digit at location I+2.
            if (NN == 0x33) {
                uint8_t h = vm->V[X] / 100;
                uint8_t t = (vm->V[X] - h * 100) / 10;
                uint8_t o = vm->V[X] - h * 100 - t * 10;
                vm->RAM[vm->I] = h;
                vm->RAM[vm->I + 1] = t;
                vm->RAM[vm->I + 2] = o;
                break;
            }

            // LD [I], Vx
            // Store registers V0 through Vx in memory starting at location I.
            // The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
            if (NN == 0x35) {
                for (int reg = 0; reg <= X; reg++) {
                    vm->RAM[vm->I + reg] = vm->V[reg];
                }
                break;
            }

            // LD Vx, [I]
            // Read registers V0 through Vx from memory starting at location I.
            // The interpreter reads values from memory starting at location I into registers V0 through Vx.
            if (NN == 0x65) {
                for (int reg = 0; reg <= X; reg++) {
                    vm->V[reg] = vm->RAM[vm->I + reg];
                }
                break;
            }
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
