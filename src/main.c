#include <stdio.h>
#include <stdlib.h>
#include "core/demuxer/demuxer.h"
#include "core/decoder/decoder.h"
#include "core/scaler/scaler.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[INFO]Usage: %s <video_file>\n", argv[0]);
        return -1;
    }

    DemuxerCtx demux;
    DecoderCtx decode;
    ScalerCtx scaler;
    int scaler_initialized = 0;

    // Initialize the format parser and find the video stream
    if (demuxer_init(&demux, argv[1]) != 0) {
        fprintf(stderr, "[ERR] Failed to initialize demuxer.\n");
        return -1;
    }

    // Grab the stream parameters extracted by the demuxer
    AVCodecParameters *pParams = demux.pFormatCtx->streams[demux.videoStream]->codecpar;

    // Pass the matching codec parameters and codec pointer into the decoder setup
    if (decoder_init(&decode, demux.pCodec, pParams) != 0) {
        fprintf(stderr, "[ERR] Failed to initialize decoder.\n");
        demuxer_close(&demux);
        return -1;
    }

    // Initialize the scaler to convert native frame format to RGB24 at the same dimensions
    if (scaler_init(&scaler, 
            pParams->width, pParams->height, pParams->format, 
            pParams->width, pParams->height, AV_PIX_FMT_RGB24) != 0) 
    {
        fprintf(stderr, "[ERR] Failed to initialize scaler.\n");
        decoder_close(&decode);
        demuxer_close(&demux);
        return -1;
    }
    scaler_initialized = 1;

    // Packet structure allocation to read data chunks into
    AVPacket *pPacket = av_packet_alloc();
    if (pPacket == NULL) 
    {

        fprintf(stderr, "[ERR] Failed to allocate packet.\n");
        scaler_close(&scaler);
        decoder_close(&decode);
        demuxer_close(&demux);
        return -1;

    }

    printf("[INFO] Successfully initialized modular media engine!\n");

    // Main execution loop -> Read compressed packets until EOF (end of file)
    while (av_read_frame(demux.pFormatCtx, pPacket) >= 0) 
    {
        // Only decode if the packet matches our targeted video stream index
        if (pPacket->stream_index == demux.videoStream) 
        {
            if (decoder_start(&decode, pPacket) == 0) 
            {
                // If a frame was successfully populated in decode.pFrame, scale it
                if (decode.pFrame->width > 0 && decode.pFrame->height > 0) 
                {
                    // Fixed warning: passed scaler.dstData pointer reference correctly to match your scaler layout
                    if (scaler_start(
                        &scaler, 
                        decode.pFrame->data, 
                        decode.pFrame->linesize, 
                        decode.pFrame->height) != 0)  
                    {

                        fprintf(stderr, "[ERR] Failed to scale frame.\n");

                    } 

                    else 
                    {
                        printf("[INFO] Scaled frame successfully to RGB24. Dest buffer: %p\n", (void*)scaler.dstData);
                    }
                }
        
            } 
            
            else 
            {
                fprintf(stderr, "[ERR] Failed to decode packet.\n");
                av_packet_free(&pPacket);
                scaler_close(&scaler);
                decoder_close(&decode);
                demuxer_close(&demux);
                return -1;
            }
        }
        av_packet_unref(pPacket);
    }

    // Standard cleanup sequence before normal program exit
    av_packet_free(&pPacket);
    if (scaler_initialized) { scaler_close(&scaler); }
    
    decoder_close(&decode);
    demuxer_close(&demux);

    return 0;
}
