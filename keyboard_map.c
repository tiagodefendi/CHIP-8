#include "keyboard_map.h"

/**
 * @brief Map SDL_Keycode to CHIP-8 key value
 * @param key SDL_Keycode from SDL event
 * @return Mapped CHIP-8 key value (0x0 to 0xF), or 0xFF if not mapped
 */
uint8_t map_keyboard_keys (SDL_Keycode key) {
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

/**
 * @brief Map CHIP-8 key value to SDL_Keycode
 * @param chip8_key CHIP-8 key value (0x0 to 0xF)
 * @return Mapped SDL_Keycode, or SDLK_UNKNOWN if not mapped
 */
uint8_t map_chip8_keys (uint8_t chip8_key) {
    switch (chip8_key) {
        // 1 - 2 - 3 - C ->
        // 1 - 2 - 3 - 4
        case 0x1: return SDLK_1;
        case 0x2: return SDLK_2;
        case 0x3: return SDLK_3;
        case 0xC: return SDLK_4;

        // 4 - 5 - 6 - D ->
        // Q - W - E - R
        case 0x4: return SDLK_q;
        case 0x5: return SDLK_w;
        case 0x6: return SDLK_e;
        case 0xD: return SDLK_r;

        // 7 - 8 - 9 - E ->
        // A - S - D - F
        case 0x7: return SDLK_a;
        case 0x8: return SDLK_s;
        case 0x9: return SDLK_d;
        case 0xE: return SDLK_f;

        // A - 0 - B - F ->
        // Z - X - C - V
        case 0xA: return SDLK_z;
        case 0x0: return SDLK_x;
        case 0xB: return SDLK_c;
        case 0xF: return SDLK_v;

        // Not mapped keys
        default: return SDLK_UNKNOWN;
    }
}

/**
 * @brief Map SDL_Scancode to CHIP-8 key value
 * @param scancode SDL_Scancode from SDL_GetKeyboardState
 * @return Mapped CHIP-8 key value (0x0 to 0xF), or 0xFF if not mapped
 */
uint8_t map_scancodes (SDL_Scancode scancode) {
    switch (scancode) {
        // 1 - 2 - 3 - C ->
        // 1 - 2 - 3 - 4
        case SDL_SCANCODE_1: return 0x1;
        case SDL_SCANCODE_2: return 0x2;
        case SDL_SCANCODE_3: return 0x3;
        case SDL_SCANCODE_4: return 0xC;

        // Q - W - E - R ->
        // 4 - 5 - 6 - D
        case SDL_SCANCODE_Q: return 0x4;
        case SDL_SCANCODE_W: return 0x5;
        case SDL_SCANCODE_E: return 0x6;
        case SDL_SCANCODE_R: return 0xD;

        // A - S - D - F ->
        // 7 - 8 - 9 - E
        case SDL_SCANCODE_A: return 0x7;
        case SDL_SCANCODE_S: return 0x8;
        case SDL_SCANCODE_D: return 0x9;
        case SDL_SCANCODE_F: return 0xE;

        // Z - X - C - V ->
        // A - 0 - B - F
        case SDL_SCANCODE_Z: return 0xA;
        case SDL_SCANCODE_X: return 0x0;
        case SDL_SCANCODE_C: return 0xB;
        case SDL_SCANCODE_V: return 0xF;

        // Not mapped keys
        default: return 0xFF;
    }
}

/**
 * @brief Map CHIP-8 key value to SDL_Scancode
 * @param chip8_key CHIP-8 key value (0x0 to 0xF)
 * @return Mapped SDL_Scancode, or SDL_SCANCODE_UNKNOWN if not mapped
 */
uint8_t map_reverse_scancodes (uint8_t chip8_key) {
    switch (chip8_key) {
        // 1 - 2 - 3 - C ->
        // 1 - 2 - 3 - 4
        case 0x1: return SDL_SCANCODE_1;
        case 0x2: return SDL_SCANCODE_2;
        case 0x3: return SDL_SCANCODE_3;
        case 0xC: return SDL_SCANCODE_4;

        // 4 - 5 - 6 - D ->
        // Q - W - E - R
        case 0x4: return SDL_SCANCODE_Q;
        case 0x5: return SDL_SCANCODE_W;
        case 0x6: return SDL_SCANCODE_E;
        case 0xD: return SDL_SCANCODE_R;

        // 7 - 8 - 9 - E ->
        // A - S - D - F
        case 0x7: return SDL_SCANCODE_A;
        case 0x8: return SDL_SCANCODE_S;
        case 0x9: return SDL_SCANCODE_D;
        case 0xE: return SDL_SCANCODE_F;

        // A - 0 - B - F ->
        // Z - X - C - V
        case 0xA: return SDL_SCANCODE_Z;
        case 0x0: return SDL_SCANCODE_X;
        case 0xB: return SDL_SCANCODE_C;
        case 0xF: return SDL_SCANCODE_V;

        // Not mapped keys
        default: return SDL_SCANCODE_UNKNOWN;
    }
}
