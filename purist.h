#ifndef PURIST_H
#define PURIST_H

#include "list.h"
#include "structs.h"

class purist
{
public:
  double xpos;
  double ypos;
  double xvel;
  double yvel;
  double jump1;
  double jump2;
  short onGround;
  short jumpTimer;
  short maxHealth;
  short health;
  short jumps;
  short roomX, roomY;

  char visited[15][13];

  short bosses[4];

  short charged, hitCounter;
  point* tPoint;
  list trail;

  short locked;

  void initialize();

  void addPoint();
  void draw();
  void drawLightning();
  void clearPath();

  void save(short roomX, short roomY);
  void load();


private:

};

#endif
