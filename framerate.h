#pragma once
#define FRAME_RATE_SCREEN_WIDTH 160
#define FRAME_RATE_SCREEN_HEIGHT 160
#include <stdint.h>
class Framerate
{
public:
    void Init();
    void Trace(double floatflametime, double fixedflametime, uint32_t *fb);
    Framerate(){};
    ~Framerate(){};

private:
    uint32_t floatframert_buffer[256];
    uint32_t fixedframert_buffer[256];
};
