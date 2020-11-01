#pragma once
#define FRAME_RATE_SCREEN_WIDTH 160
#define FRAME_RATE_SCREEN_HEIGHT 160
class Framerate
{
public:
    void Init();
    void Trace(double floatflametime, double fixedflametime);
    Framerate(){};
    ~Framerate(){};
};
