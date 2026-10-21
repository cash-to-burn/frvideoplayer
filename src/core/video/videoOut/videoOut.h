#ifndef VIDEOOUT_H
#define VIDEOOUT_H

#include <SDL2/SDL.h>

typedef struct {
    int height;
    int width;
    
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;

} VideoOutCtx;


int videoOut_init(VideoOutCtx *ctx, int height, int width, const char *title);
int videOut_FrameRender(VideoOutCtx *ctx, uint8_t *pixels, int SizeOfLine);
int videoOut_PollEvents(void);
void videoOut_close(VideoOutCtx *ctx);


#endif