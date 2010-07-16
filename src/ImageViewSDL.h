/* 
 * File:   ImageViewSDL.h
 * Author: blueck
 *
 * Created on 1. Juli 2010, 13:42
 */
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include <QtGui/QImage>

#ifndef IMAGEVIEWSDL_H
#define	IMAGEVIEWSDL_H

class ImageViewSDL
{
public:
    ImageViewSDL();
    virtual ~ImageViewSDL();
    int init();
private:

};

#endif	/* IMAGEVIEWSDL_H */

