#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "structs.h"

class Explosion
{
public:
  Explosion(short x, short y, double s, unsigned char r=255, unsigned char g=255, unsigned char b=255, unsigned char a=255);
  short update();
  void draw();

private:

  short xpos, ypos;
  double size, maxSize;
  Color color;
};

#endif
