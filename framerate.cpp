#include "framerate.h"
#include <math.h>
#include <stdio.h>

void Framerate::Init()
{
    printf("init\n");
}
void Framerate::Trace(double floatflametime, double fixedflametime)
{
    printf("Trace : t1 : %f, t2 : %f \n", floatflametime, fixedflametime);
}
