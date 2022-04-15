#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <cmath>
#include <SDL.h>
#include "button.h"

using namespace std;

void button::initialize(short xpos, short ypos, char* filename)
{
  image = SDL_LoadBMP(filename);
  img.x = 0;
  img.y = 0;
  img.w = image->w;
  img.h = image->h;
  target.x = xpos;
  target.y = ypos;
  target.w = img.w;
  target.h = img.h;
}

void button::initialize(short xpos, short ypos)
{
  target.x = xpos;
  target.y = ypos;
}

void button::draw(SDL_Surface* s)
{
  SDL_BlitSurface(image, &img, s, &target);
  if(active == 1)
  {
    short x = 0;
    for(x = target.x; x < target.w + target.x; ++x)
    {
      *((Uint32 *)s->pixels+(target.y)*s->pitch/4+(x))= 65280;
      *((Uint32 *)s->pixels+(target.y+target.h-1)*s->pitch/4+(x))= 65280;
    }
    for(x = target.y; x < target.h + target.y; ++x)
    {
      *((Uint32 *)s->pixels+(x)*s->pitch/4+(target.x))= 65280;
      *((Uint32 *)s->pixels+(x)*s->pitch/4+(target.x+target.w-1))= 65280;
    }
  }
  else if(over == 1)
  {
    short x = 0;
    for(x = target.x; x < target.w + target.x; ++x)
    {
      *((Uint32 *)s->pixels+(target.y)*s->pitch/4+(x))= 32768;
      *((Uint32 *)s->pixels+(target.y+target.h-1)*s->pitch/4+(x))= 32768;
    }
    for(x = target.y; x < target.h + target.y; ++x)
    {
      *((Uint32 *)s->pixels+(x)*s->pitch/4+(target.x))= 32768;
      *((Uint32 *)s->pixels+(x)*s->pitch/4+(target.x+target.w-1))= 32768;
    }
  }
}

void button::draw(SDL_Surface* s, short r, short g, short b)
{
  SDL_BlitSurface(image, &img, s, &target);
  if(active == 1)
  {
    short x = 0;
    for(x = target.x; x < target.w + target.x; ++x)
    {
      *((Uint32 *)s->pixels+(target.y)*s->pitch/4+(x))= 32768;
      *((Uint32 *)s->pixels+(target.y+target.h-1)*s->pitch/4+(x))= 32768;
    }
    for(x = target.y; x < target.h + target.y; ++x)
    {
      *((Uint32 *)s->pixels+(x)*s->pitch/4+(target.x))= 32768;
      *((Uint32 *)s->pixels+(x)*s->pitch/4+(target.x+target.w-1))= 32768;
    }
  }
  else if(over == 1)
  {
    short x = 0;
    Uint32 color = ((r<<8) + g<<8) + b;
    for(x = target.x; x < target.w + target.x; ++x)
    {
      *((Uint32 *)s->pixels+(target.y)*s->pitch/4+(x))= color;
      *((Uint32 *)s->pixels+(target.y+target.h-1)*s->pitch/4+(x))= color;
    }
    for(x = target.y; x < target.h + target.y; ++x)
    {
      *((Uint32 *)s->pixels+(x)*s->pitch/4+(target.x))= color;
      *((Uint32 *)s->pixels+(x)*s->pitch/4+(target.x+target.w-1))= color;
    }
  }

}

void button::toggleActive()
{
  if(active == 1)
    active = 0;
  else
   active = 1;
}

void button::setActive()
{
  active = 1;
}

void button::setInactive()
{
  active = 0;
}

short button::getActive()
{
  return active;
}

short button::getCollision(short xpos, short ypos)
{
  over = 0;
  if(!(xpos >=  target.x))
    return 0;
  if(!(xpos <  target.x+target.w))  
    return 0;
  if(!(ypos >=  target.y))
    return 0;
  if(!(ypos <  target.y+target.h))  
    return 0;
  over = 1;
  return 1;
}
