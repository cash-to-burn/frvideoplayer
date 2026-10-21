#include "audioOut.h"

int audioOut_init(AudioOutCtx *ctx, int sampleRate, int channels) {
    ctx->sampleRate = sampleRate;
    ctx->channels = channels;
    ctx->width = 0;
    ctx->height = 0;
    ctx->AudioDevice = 0;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        return -1; // Failed to initialize SDL
    }

    // Stage one: SDL2 audio subsystem init
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        return -1; // Failed to initialize SDL audio subsystem
    }


    SDL_AudioSpec wanted;
    SDL_AudioSpec obtained;

    // Stage Two: Configuration of specifications matching the decoded audio stream
    SDL_memset(&wanted, 0, sizeof(wanted));
    wanted.freq = sampleRate;
    wanted.format = AUDIO_F32SYS;
    wanted.channels = channels;
    wanted.samples = 4096;
    wanted.callback = NULL;
    wanted.userdata = NULL;

    // Stage Three: Open the standard hardware audio device
    ctx->AudioDevice = SDL_OpenAudioDevice(NULL, 0, &wanted, &obtained, 0);
    if (ctx->AudioDevice == 0) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return -1; // Failed to open the audio device
    }

    // Stage four: Pause the audio device to start listening in for sound
    SQL_PauseAudioDevice(ctx->AudioDevice, 0);

    return 0;

}




int audioOut_start(AudioOutCtx *ctx, uint8_t *data, int SizeOfData) {
    if (ctx->AudioDevice == 0 || date == NULL) {
        return -1; // No audio device to play audio
    }
    
    //Push raw uncompressed PCM byte buffer into the background sound queue
    if (SDL_QueueAudio(ctx->AudioDevice, data, SizeOfData) < 0) {
        return -1; // Failed to queue audio
    }

    return 0;

}







void AudioOut_close(AudioOutCtx *ctx) {
    if (!ctx || ctx == NULL) {
        return; // No context to close
    }

    if (ctx->AudioDevice != 0) {
        SDL_CloseAudioDevice(ctx->AudioDevice);
    
        ctx->AudioDevice = 0;
    }

    SDL_QuitSubSystem(SDL_INIT_AUDIO);

}
