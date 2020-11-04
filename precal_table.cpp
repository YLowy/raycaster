#include "precal_table.h"
#include <math.h>
#include <stdio.h>


void Precal_table::Precal_table_Init()
{
    printf("--Precal Init---\n");
    printf("--Precal Init gsin() gcos()---\n");

    for (int i = 0; i < 256; i++) {
        g_sin[i] = static_cast<uint8_t>(256.0f * sin(i / 1024.0f * 2 * M_PI));
        g_cos[i] = static_cast<uint8_t>(256.0f * cos(i / 1024.0f * 2 * M_PI));
    }
    for (int i = 0; i < 256; i++) {
        g_tan[i] = static_cast<uint16_t>((256.0f * tan(i * M_PI_2 / 256.0f)));
        g_cotan[i] = static_cast<uint16_t>((256.0f / tan(i * M_PI_2 / 256.0f)));
    }
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        float deltaAngle = atanf(((int16_t) i - SCREEN_WIDTH / 2.0f) /
                                 (SCREEN_WIDTH / 2.0f) * M_PI / 4);
        int16_t da = static_cast<int16_t>(deltaAngle / M_PI_2 * 256.0f);
        if (da < 0) {
            da += 1024;
        }
        g_deltaAngle[i] = static_cast<uint16_t>(da);
    }

    for (int i = 0; i < 256; i++) {
        g_nearHeight[i] = static_cast<uint8_t>(
            (INV_FACTOR_INT / (((i << 2) + MIN_DIST) >> 2)) >> 2);
        g_farHeight[i] = static_cast<uint8_t>(
            (INV_FACTOR_INT / (((i << 5) + MIN_DIST) >> 5)) >> 5);
    }

    for (int i = 0; i < 256; i++) {
        auto txn =
            ((INV_FACTOR_INT / (((i * 4.0f) + MIN_DIST) / 4.0f)) / 4.0f) * 2.0f;
        if (txn != 0) {
            g_nearStep[i] = (256 / txn) * 256;
        }
        auto txf =
            ((INV_FACTOR_INT / (((i * 32.0f) + MIN_DIST) / 32.0f)) / 32.0f) *
            2.0f;
        if (txf != 0) {
            g_farStep[i] = (256 / txf) * 256;
        }
    }

    for (int i = 1; i < 256; i++) {
        auto txs = ((INV_FACTOR_INT / (float) (i / 2.0f)));
        auto ino = (txs - SCREEN_HEIGHT) / 2;
        g_overflowStep[i] = (256 / txs) * 256;
        g_overflowOffset[i] = ino * (256 / txs) * 256;
    }

}

Precal_table::Precal_table(){};
Precal_table::~Precal_table(){};
