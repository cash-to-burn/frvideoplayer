#ifndef AUDIOOUT_H
#define AUDIOOUT_H

#include <SDL2/SDL.h>

typedef struct {
    int width;
    int height;

    int sampleRate;
    int  channels;
    SDL_AudioDeviceID AudioDevice;

} AudioOutCtx;

int audioOut_init(AudioOutCtx *ctx, int sampleRate, int channels);
int audioOut_start(AudioOutCtx *ctx, uint8_t *data, int SizeOfData);
void audioOut_close(AudioOutCtx *ctx);

#endif