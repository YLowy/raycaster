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
    printf("Trace : t1 : %f, t2 : %f \n", floatflametime, fixedflametime);
    for (int i = 0; i < FRAME_RATE_SCREEN_WIDTH * FRAME_RATE_SCREEN_HEIGHT;
         i++) {
        fb[i] = 99999999;
        if ((i > 160 * 80) && (i < 160 * 81))
            fb[i] = 0;
    }
}
