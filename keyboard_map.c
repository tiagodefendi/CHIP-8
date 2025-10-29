#include "keyboard_map.h"

/**
 * @brief Map SDL_Keycode to CHIP-8 key value
 * @param key SDL_Keycode from SDL event
 * @return Mapped CHIP-8 key value (0x0 to 0xF), or 0xFF if not mapped
 */
uint8_t map_keys (SDL_Keycode key) {
    switch (key) {
        // 1 - 2 - 3 - 4 ->
        // 1 - 2 - 3 - C
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_4: return 0xC;

        // Q - W - E - R ->
        // 4 - 5 - 6 - D
        case SDLK_q: return 0x4;
        case SDLK_w: return 0x5;
        case SDLK_e: return 0x6;
        case SDLK_r: return 0xD;

        // A - S - D - F ->
        // 7 - 8 - 9 - E
        case SDLK_a: return 0x7;
        case SDLK_s: return 0x8;
        case SDLK_d: return 0x9;
        case SDLK_f: return 0xE;

        // Z - X - C - V ->
        // A - 0 - B - F
        case SDLK_z: return 0xA;
        case SDLK_x: return 0x0;
        case SDLK_c: return 0xB;
        case SDLK_v: return 0xF;

        // Not mapped keys
        default: return 0xFF;
    }
}
