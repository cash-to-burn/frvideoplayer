#include "videoOut.h"

int videoOut_init(VideoOutCtx *ctx, int height, int width, const char *title) {
    ctx->height = height;
    ctx->width = width;
    ctx->pWindow = NULL;
    ctx->pRenderer = NULL;
    ctx->pTexture = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1; // Failed to initialize SDL
    }

    // Create the window
    ctx->pWindow = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        width, height, 
        SDL_WINDOW_SHOWN
    );

    if (ctx->pWindow == NULL || !ctx->pWindow) {
        SDL_Quit();
        return -1; // Failed to create the window
    }

    ctx->pRenderer = SDL_CreateRenderer(ctx->pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (ctx->pRenderer == NULL || !ctx->pRenderer) {
        SDL_DestroyWindow(ctx->pWindow);
        SDL_Quit();
        return -1; // Failed to create the renderer
    }

    // Create the texture
    ctx->pTexture = SDL_CreateTexture(ctx->pRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (ctx->pTexture == NULL || !ctx->pTexture) {
        SDL_DestroyRenderer(ctx->pRenderer);
        SDL_DestroyWindow(ctx->pWindow);
        SDL_Quit();
        return -1; // Failed to create the texture
    }

    return 0;

}


int videOut_FrameRender(VideoOutCtx *ctx, uint8_t *pixels, int SizeOfLine) {
    // Stage one: Update the texture
    if(SDL_UpdateTexture(ctx->pTexture, NULL, pixels, SizeOfLine) < 0) {
        return -1; // Failed to update the texture
    }

    // Stage two: Render the texture to the window
    SDL_RenderClear(ctx->pRenderer);

    // Stage three: Map the new canvas layout on the internal window matrix
    SDL_RenderCopy(ctx->pRenderer, ctx->pTexture, NULL, NULL);

    // Stage four: Force the GPU card layer to display the completed frame
    SDL_RenderPresent(ctx->pRenderer);

    return 0;
}


int videoOut_PollEvents(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return -1; // User requested to quit the program
        }
    }
    
    return 0;
}

void videoOut_close(VideoOutCtx *ctx) {
    if (!ctx || ctx == NULL) {
        return; // No context to close
    }

    if (ctx->pTexture) {
        SDL_DestroyTexture(ctx->pTexture);
    }

    if (ctx->pRenderer) {
        SDL_DestroyRenderer(ctx->pRenderer);
    }

    if (ctx->pWindow) {
        SDL_DestroyWindow(ctx->pWindow);
    }

    SDL_Quit();
}