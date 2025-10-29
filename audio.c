#include "audio.h"

void play_sound(SDL_AudioDeviceID audio_device, uint8_t* sound_buffer) {
    SDL_QueueAudio(audio_device, sound_buffer, sizeof(sound_buffer));
}

void stop_sound(SDL_AudioDeviceID audio_device) {
    SDL_ClearQueuedAudio(audio_device);
}
