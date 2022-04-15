#include <cstdlib> 
#include <iostream>
#include <cmath>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "explosion.h"

Explosion::Explosion(short x, short y, double s, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  xpos = x;
  ypos = y;
  size = 0;
  maxSize = s;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;
}
short Explosion::update()
{
  if(size >= maxSize)
    return -1;
  size+=.5;
  return 0;
}
void Explosion::draw()
{
  glColor4ub(color.r, color.g, color.b, color.a*(1-size/maxSize));
  glBegin(GL_QUADS);
    glVertex2d(xpos-size-3, ypos-size);
    glVertex2d(xpos-size-3, ypos+size);
    glVertex2d(xpos-size, ypos+size);
    glVertex2d(xpos-size, ypos-size);

    glVertex2d(xpos-size, ypos+size+3);
    glVertex2d(xpos+size, ypos+size+3);
    glVertex2d(xpos+size, ypos+size);
    glVertex2d(xpos-size, ypos+size);

    glVertex2d(xpos+size+3, ypos+size);
    glVertex2d(xpos+size+3, ypos-size);
    glVertex2d(xpos+size, ypos-size);
    glVertex2d(xpos+size, ypos+size);

    glVertex2d(xpos+size, ypos-size-3);
    glVertex2d(xpos-size, ypos-size-3);
    glVertex2d(xpos-size, ypos-size);
    glVertex2d(xpos+size, ypos-size);
  glEnd();
  glClear(GL_DEPTH_BUFFER_BIT);
}

