#ifndef SCALER_H
#define SCALER_H
#include <libavformat/avformat.h>
#include <libavutil/pixfmt.h>


typedef struct {
    struct SwsContext *pSwsCtx; 
    uint8_t  *dstData[4];
    int dstLinesize[4];
    enum AVPixelFormat dstPixFormat;
    int dstWidth;
    int dstHeight;
} ScalerCtx;


int scaler_init(ScalerCtx *ctx, int ScreenWidth, int ScreenHeight, enum AVPixelFormat PixelFormat, int dstWidth, int dstHeight, enum AVPixelFormat dstPixFormat);
int scaler_start(ScalerCtx *ctx, uint8_t *srcData[], int srcLinesize[], int srcHeight);
void scaler_close(ScalerCtx *ctx);

#endif
