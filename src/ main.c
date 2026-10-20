#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include <stdio.h>
#include <stdlib.h>

// #define WIDTH 640
// #define HEIGHT 480


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <video_file>\n", argv[0]);
        return -1;

    }
    // Context format creation
    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
    
    const AVCodec *pCodec = NULL;

    AVFrame *pFrame = NULL;
    
    int videoStream = -1;


    // Finding the first  video stream and decoder


    // allocate the codec context needed for decoder
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (pCodecCtx == NULL) {
        avformat_close_input(&pFormatCtx); 
        return -1; // Failed to allocate the codec context
    }



    // Copy the codec parameters from the stream to the codec context
    if (avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar) < 0) {
        avformat_close_input(&pFormatCtx); 
        return -1; // Failed to copy the codec parameters
    }

    // Open the decoder
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        avcodec_free_context(&pCodecCtx); 
        avformat_close_input(&pFormatCtx); 
        return -1; // Failed to open the decoder
    }
    
    pFrame = av_frame_alloc();
    if (pFrame == NULL) {
        avcodec_free_context(&pCodecCtx); 
        avformat_close_input(&pFormatCtx); 
        return -1; // Failed to allocate the frame
    }
}
