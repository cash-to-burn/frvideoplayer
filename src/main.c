#include <stdio.h>
#include <stdlib.h>
#include "core/demuxer/demuxer.h"
#include "core/decoder/decoder.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <video_file>\n", argv[0]);
        return -1;
    }

    DemuxerCtx demux;
    DecoderCtx decode;

    // Initialize the format parser and find the video stream
    if (demuxer_init(&demux, argv[1]) != 0) {
        fprintf(stderr, "Failed to initialize demuxer.\n");
        return -1;
    }

    // Grab the stream parameters extracted by the demuxer
    AVCodecParameters *pParams = demux.pFormatCtx->streams[demux.videoStream]->codecpar;

    // Pass the matching codec parameters and codec pointer into the decoder setup
    if (decoder_init(&decode, demux.pCodec, pParams) != 0) {
        fprintf(stderr, "Failed to initialize decoder.\n");
        demuxer_close(&demux);
        return -1;
    }

    printf("Successfully initialized modular media engine!\n");

    // Clean up allocated subsystem frames and contexts before exiting
    decoder_close(&decode);
    demuxer_close(&demux);

    return 0;
}
