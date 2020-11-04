#pragma once

#include <stdint.h>
#include "raycaster.h"
#ifdef TABLES_320
#define HAS_TABLES
#endif  // TABLES_320
class Precal_table
{
public:
    uint16_t g_tan[256];
    uint16_t g_cotan[256];
    uint8_t g_sin[256];
    uint8_t g_cos[256];
    uint8_t g_nearHeight[256];
    uint8_t g_farHeight[256];
    uint16_t g_nearStep[256];
    uint16_t g_farStep[256];
    uint16_t g_overflowOffset[256];
    uint16_t g_overflowStep[256];
    uint16_t g_deltaAngle[SCREEN_WIDTH];

    void Precal_table_Init();

    Precal_table();
    ~Precal_table();
};
