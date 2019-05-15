#pragma once

//Output gaat steeds langs 1
#define BMD_DEVICE_1 9
#define BMD_DEVICE_2 1

#define OUTPUT_DEVICE 1 // kies of bovenstaand device 1 of device 2 gebruikt wordt

#define DISPLAY_MODE BMDDisplayMode::bmdModeHD1080p50
#define PIXEL_MODE BMDPixelFormat::bmdFormat8BitYUV

#define WIDTH 1920
#define HEIGHT 1080
#define FRAME_RATE 50 //fps
#define FRAME_TIME 1000/FRAME_RATE

//enable chronometer
#define EN_CHRONO 0

#define IMAGE_CHANNELS 3