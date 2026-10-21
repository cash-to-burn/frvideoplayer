#include "scaler.h"
#include <stdlib.h> // Fixed typo: <stdlib> -> <stdlib.h>

#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>


int scaler_init(ScalerCtx *ctx, int ScreenWidth, int ScreenHeight, enum AVPixelFormat PixelFormat, int dstWidth, int dstHeight, enum AVPixelFormat dstPixFormat) {


    ctx->dstHeight = dstHeight;
    ctx->dstWidth = dstWidth;
    ctx->dstPixFormat = dstPixFormat;
    ctx->pSwsCtx = NULL;

    int ret = 0;

    // Allocate the scaling context
    ctx->pSwsCtx = sws_getContext(ScreenWidth, 
        ScreenHeight, PixelFormat, 
        dstWidth, dstHeight, 
        dstPixFormat, 
        SWS_BICUBIC, 
        NULL, NULL, NULL
    );

    if (ctx->pSwsCtx == NULL) {
        return -1; // Failed to allocate the scaling context
    }
    
    ret = av_image_alloc(ctx->dstData, ctx->dstLinesize, dstWidth, dstHeight, dstPixFormat, 1);
    if (ret < 0) {
        return -1; // Failed to allocate the destination image
    }

    return 0;

}

int scaler_start(ScalerCtx *ctx, uint8_t *srcData[], int srcLinesize[], int srcHeight) {
    // Fixed warning: Casted srcData to expected pointer layout signature to match FFmpeg definitions
    int ret = sws_scale(ctx->pSwsCtx, 
        (const uint8_t *const *)srcData, srcLinesize, 0, srcHeight, 
        ctx->dstData, ctx->dstLinesize
    );

    if (ret < 0) {
        return -1; // Failed to scale the image
    }

    return 0;
}

void scaler_close(ScalerCtx *ctx) {
    if (ctx->pSwsCtx) {
        sws_freeContext(ctx->pSwsCtx);
    }

    if (ctx->dstData[0]) {
        av_freep(&ctx->dstData[0]);
    }
}
