/* 
 * File:   ImageViewSDL.cc
 * Author: blueck
 * 
 * Created on 1. Juli 2010, 13:42
 */

#include "ImageViewSDL.h"

ImageViewSDL::ImageViewSDL()
{
}


ImageViewSDL::~ImageViewSDL()
{
}

int ImageViewSDL::init()
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 480;
    SDL_Surface* screen;
    screen = SDL_SetVideoMode(640, 480, 0, 0);
    SDL_Overlay* overlay;
    overlay = SDL_CreateYUVOverlay(640, 480, SDL_YV12_OVERLAY, screen);
    SDL_LockYUVOverlay(overlay);
    QImage* image = new QImage("/home/blueck/pics/test.jpg");
    memcpy(overlay->pixels[0], y_video_data, my_overlay->pitches[0]);
    memcpy(overlay->pixels[1], u_video_data, my_overlay->pitches[1]);
    memcpy(overlay->pixels[2], v_video_data, my_overlay->pitches[2]);


    SDL_UnlockYUVOverlay(overlay);
    SDL_DisplayYUVOverlay(overlay, rect);
    return 0;
}

