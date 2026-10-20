#include "demuxer.h"
#include <stdio.h>


int demuxer_init(DemuxerCtx *ctx, const char *filename) {
   // Context format creation
    ctx->pFormatCtx = NULL;
    ctx->pCodecCtx = NULL;
    ctx->pCodec = NULL;
    ctx->videoStream = -1;

    // Input File Opening
    if (avformat_open_input(&ctx->pFormatCtx, filename, NULL, NULL) != 0) { 
        return -1; 
    } // Failure in file opening

    // Stream information retrieval
    if (avformat_find_stream_info(ctx->pFormatCtx, NULL) < 0) {
        avformat_close_input(&ctx->pFormatCtx); 
        return -1; 
    } // Failure in finding stream info

    // Dumping the format of the file onto stderr
    av_dump_format(ctx->pFormatCtx, 0, filename, 0);  


    ctx->videoStream = av_find_best_stream(ctx->pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &ctx->pCodec, 0);
    if (ctx->videoStream == -1) {
        avformat_close_input(&ctx->pFormatCtx); 
        return -1; // Video stream failed to be found
    } 

    return 0;

}

void demuxer_close(DemuxerCtx *ctx) {
    if (ctx && ctx->pFormatCtx) {
        avformat_close_input(&ctx->pFormatCtx); 
    }

}