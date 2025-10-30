#include "audio.h"

void play_sound(SDL_AudioDeviceID audio_device, uint8_t* sound_buffer, int size) {
    SDL_QueueAudio(audio_device, sound_buffer, size);
}

void stop_sound(SDL_AudioDeviceID audio_device) {
    SDL_ClearQueuedAudio(audio_device);
}
