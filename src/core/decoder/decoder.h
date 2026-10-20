#infdef DECODER_H
#define DECODER_H
#include <libavcodec/avcodec.h>

typedef struct {
    AVCodecContext *pCodecCtx;
    AVFrame *pFrame;
} DecoderCtx;

int decoder_init(DecoderCtx *ctx, const AVcodec *pCodec, AVCodecParameters *pCodecParams);
void decoder_close(DecoderCtx *ctx);

#endif