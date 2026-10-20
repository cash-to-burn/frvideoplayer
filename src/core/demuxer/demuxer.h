#ifndef DEMUXER_H
#define DEMUXER_H

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

typedef struct {
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    const AVCodec *pCodec;
    int videoStream;
} DemuxerCtx;


int demuxer_init(DemuxerCtx *ctx, const char *filename);
void demuxer_close(DemuxerCtx *ctx);

#endif