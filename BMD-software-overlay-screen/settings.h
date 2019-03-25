#pragma once

#define BMD_DEVICE 9

#define DISPLAY_MODE BMDDisplayMode::bmdModeHD720p50
#define PIXEL_MODE BMDPixelFormat::bmdFormat8BitBGRA

#define WIDTH 1280
#define HEIGHT 720
#define FRAME_RATE 50 //fps
#define FRAME_TIME 1000/FRAME_RATE

//enable chronometer
#define EN_CHRONO 0

//image settings
#define NO_SIGNAL_IMG "nosignal_720.tga"
#define IMAGE_NAME "../../720_Test_small.tga"
#define IMAGE_CHANNELS 4

#define NUM_THREADS 2