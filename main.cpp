#include <SDL.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

#include "game.h"
#include "raycaster.h"
#include "raycaster_fixed.h"
#include "raycaster_float.h"
#include "renderer.h"

#include "bench.c"
#include "bench.h"

#include "precal_table.h"

#include "framerate.h"

using namespace std;

static void DrawBuffer(SDL_Renderer *sdlRenderer,
                       SDL_Texture *sdlTexture,
                       uint32_t *fb,
                       int dx)
{
    int pitch = 0;
    void *pixelsPtr;
    if (SDL_LockTexture(sdlTexture, NULL, &pixelsPtr, &pitch)) {
        throw runtime_error("Unable to lock texture");
    }
    memcpy(pixelsPtr, fb, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
    SDL_UnlockTexture(sdlTexture);
    SDL_Rect r;
    r.x = dx * SCREEN_SCALE;
    r.y = 0;
    r.w = SCREEN_WIDTH * SCREEN_SCALE;
    r.h = SCREEN_HEIGHT * SCREEN_SCALE;
    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &r);
}

static void DrawFrameBuffer(SDL_Renderer *sdlRenderer,
                            SDL_Texture *sdlTexture,
                            uint32_t *fb,
                            int dx)
{
    int pitch = 0;
    void *pixelsPtr;
    if (SDL_LockTexture(sdlTexture, NULL, &pixelsPtr, &pitch)) {
        throw runtime_error("Unable to lock texture");
    }
    memcpy(
        pixelsPtr, fb,
        FRAME_RATE_SCREEN_WIDTH * FRAME_RATE_SCREEN_HEIGHT * sizeof(uint32_t));
    SDL_UnlockTexture(sdlTexture);
    SDL_Rect r;
    r.x = dx * SCREEN_SCALE;
    r.y = 0;
    r.w = FRAME_RATE_SCREEN_WIDTH * SCREEN_SCALE;
    r.h = FRAME_RATE_SCREEN_HEIGHT * SCREEN_SCALE;
    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &r);
}

static bool ProcessEvent(const SDL_Event &event,
                         int *moveDirection,
                         int *rotateDirection)
{
    if (event.type == SDL_QUIT) {
        return true;
    } else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
               event.key.repeat == 0) {
        auto k = event.key;
        auto p = event.type == SDL_KEYDOWN;
        switch (k.keysym.sym) {
        case SDLK_ESCAPE:
            return true;
            break;
        case SDLK_UP:
            *moveDirection = p ? 1 : 0;
            break;
        case SDLK_DOWN:
            *moveDirection = p ? -1 : 0;
            break;
        case SDLK_LEFT:
            *rotateDirection = p ? -1 : 0;
            break;
        case SDLK_RIGHT:
            *rotateDirection = p ? 1 : 0;
            break;
        default:
            break;
        }
    }
    return false;
}

Precal_table pctable;
int main(int argc, char *args[])
{
    //--- precal Table
    pctable.Precal_table_Init();
    //---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        SDL_Window *sdlWindow =
            SDL_CreateWindow("RayCaster [floating-point s. fixed-point]",
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_SCALE * (SCREEN_WIDTH * 2 + 1),
                             SCREEN_SCALE * SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        //---
        SDL_Window *sdlframertWindow = SDL_CreateWindow(
            "frame rate [fixed-point vs. floating-point]",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_SCALE * (FRAME_RATE_SCREEN_WIDTH),
            SCREEN_SCALE * FRAME_RATE_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        //---

        if (sdlWindow == NULL || sdlframertWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n",
                   SDL_GetError());
        } else {
            Game game;
            double t1 = 0.0, t2 = 0.0, t3 = 0.0;
            RayCasterFloat floatCaster;
            Renderer floatRenderer(&floatCaster);
            uint32_t floatBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
            RayCasterFixed fixedCaster;
            Renderer fixedRenderer(&fixedCaster);
            uint32_t fixedBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
            //===
            Framerate framert;
            framert.Init();
            uint32_t framerateBuffer[FRAME_RATE_SCREEN_WIDTH *
                                     FRAME_RATE_SCREEN_HEIGHT];
            //===



            int moveDirection = 0;
            int rotateDirection = 0;
            bool isExiting = false;
            const static auto tickFrequency = SDL_GetPerformanceFrequency();
            auto tickCounter = SDL_GetPerformanceCounter();
            SDL_Event event;

            SDL_Renderer *sdlRenderer =
                SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
            //---
            SDL_Renderer *sdlframeRender = SDL_CreateRenderer(
                sdlframertWindow, -1, SDL_RENDERER_ACCELERATED);
            //---


            SDL_Texture *fixedTexture = SDL_CreateTexture(
                sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
            SDL_Texture *floatTexture = SDL_CreateTexture(
                sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
            //------
            SDL_Texture *framerateTexture = SDL_CreateTexture(
                sdlframeRender, SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_STREAMING, FRAME_RATE_SCREEN_WIDTH,
                FRAME_RATE_SCREEN_HEIGHT);
            //------

            while (!isExiting) {
                t1 = tvgetf();
                floatRenderer.TraceFrame(&game, floatBuffer);
                t2 = tvgetf();
                fixedRenderer.TraceFrame(&game, fixedBuffer);
                t3 = tvgetf();
                DrawBuffer(sdlRenderer, fixedTexture, fixedBuffer, 0);
                DrawBuffer(sdlRenderer, floatTexture, floatBuffer,
                           SCREEN_WIDTH + 1);
                //---


                framert.Trace(t2 - t1, t3 - t2, framerateBuffer);
                DrawFrameBuffer(sdlframeRender, framerateTexture,
                                framerateBuffer, 0);
                //---
                SDL_RenderPresent(sdlRenderer);
                SDL_RenderPresent(sdlframeRender);

                if (SDL_PollEvent(&event)) {
                    isExiting =
                        ProcessEvent(event, &moveDirection, &rotateDirection);
                }
                const auto nextCounter = SDL_GetPerformanceCounter();
                const auto seconds = (nextCounter - tickCounter) /
                                     static_cast<float>(tickFrequency);
                tickCounter = nextCounter;
                game.Move(moveDirection, rotateDirection, seconds);
            }
            SDL_DestroyTexture(floatTexture);
            SDL_DestroyTexture(fixedTexture);
            SDL_DestroyTexture(framerateTexture);
            SDL_DestroyRenderer(sdlRenderer);
            SDL_DestroyRenderer(sdlframeRender);
            SDL_DestroyWindow(sdlWindow);
            SDL_DestroyWindow(sdlframertWindow);
        }
    }

    SDL_Quit();
    return 0;
}
