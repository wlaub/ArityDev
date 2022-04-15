#ifndef STRUCTS_H
#define STRUCTS_H

#include "list.h"

typedef struct
{
  double x;
  double y;
} point;

typedef struct
{
  float x;
  float y;
  short amount;
  short killed;
} Anode;

typedef struct
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} Color;

class bullet
{
public:
  double x;
  double y;
  double xvel;
  double yvel;
  short active;
};

#endif
