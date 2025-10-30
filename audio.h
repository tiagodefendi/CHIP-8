#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>

void play_sound(SDL_AudioDeviceID audio_device, uint8_t* sound_buffer, int size);
void stop_sound(SDL_AudioDeviceID audio_device);
