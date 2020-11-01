#include "framerate.h"
#include <math.h>
#include <stdio.h>

void Framerate::Init()
{
    printf("Framerate init\n");
    for (int i = 0; i < 256; i++) {
        floatframert_buffer[0] = 0;
        fixedframert_buffer[0] = 0;
    }
}
void Framerate::Trace(double floatflametime,
                      double fixedflametime,
                      uint32_t *fb)
{
    for (int i = 0; i < FRAME_RATE_SCREEN_WIDTH * FRAME_RATE_SCREEN_HEIGHT;
         i++) {
        fb[i] = 99999999;
        if ((i > 160 * 80) && (i < 160 * 81))
            fb[i] = 0;
    }
    uint8_t floatpoint = (uint8_t)(floatflametime * 100000);
    uint8_t fixedpoint = (uint8_t)(fixedflametime * 100000);
    static int counter = 0;
    // printf("Trace : t1 : %d, t2 : %d   :%d \n", floatpoint,
    // fixedpoint,counter++);

    floatframert_buffer[buffer_count] = floatpoint;
    fixedframert_buffer[buffer_count] = fixedpoint;
    if (buffer_count == 255)
        buffer_count = 0;
    else
        buffer_count++;

    for (int i = 0; i < 60; i++) {
        int tmp = (int) buffer_count - i;
        if (tmp < 0)
            tmp += 255;

        fb[160 * (80 - floatframert_buffer[tmp]) + ((60 - i) << 1)] = 0;
        fb[160 * (160 - fixedframert_buffer[tmp]) + ((60 - i) << 1)] = 0;
    }
}
