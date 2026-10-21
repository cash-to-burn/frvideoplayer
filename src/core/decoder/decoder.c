#include "decoder.h"
#include <stdio.h>


int decoder_init(DecoderCtx *ctx, const AVCodec *pCodec, AVCodecParameters *pCodecParams) {
    ctx->pCodecCtx = avcodec_alloc_context3(pCodec);
    if (ctx->pCodecCtx == NULL) {
        return -1; // Failed to allocate the codec context
    }

    // Copy the codec parameters from the stream to the codec context
    if (avcodec_parameters_to_context(ctx->pCodecCtx, pCodecParams) < 0) {
        return -1; // Failed to copy the codec parameters
    }

    // Open the decoder
    if (avcodec_open2(ctx->pCodecCtx, pCodec, NULL) < 0) {
        return -1; // Failed to open the decoder
    }

    ctx->pFrame = av_frame_alloc();
    if (ctx->pFrame == NULL) {
        return -1; // Failed to allocate the frame
    }
    
    return 0;
}


int decoder_start(DecoderCtx *ctx, AVPacket *pPacket) {
    int ret = avcodec_send_packet(ctx->pCodecCtx, pPacket);
    if (ret < 0) {
        return -1; // Failed to send the packet to the decoder
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(ctx->pCodecCtx, ctx->pFrame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return 0; // No more frames to decode
        } else if (ret < 0) {
            return -1; // Failed to decode the frame
        }

        // Action inside loop: Must execute for every frame received before the loop cycles
        printf("Decoded frame at %ld\n", ctx->pFrame->pts);
        av_frame_unref(ctx->pFrame);
    }

    return 0;
}




void decoder_close(DecoderCtx *ctx) {
    if (ctx->pFrame == NULL) {
        avcodec_free_context(&ctx->pCodecCtx); 
    }
    if (ctx->pFrame) {
        av_frame_free(&ctx->pFrame); 
    }
}
