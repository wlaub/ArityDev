#ifndef BOSS_H
#define BOSS_H

#include "purist.h"
#include <fstream>

using namespace std;

class boss
{
public:

  void init();

  void refresh();
  void load(ifstream* fin);
  void save(ofstream* fout);
  void reset();

  short checkBlock(short xpos, short ypos);
  void collide(purist* dood);
  void draw();

  void drawBlock(short xpos, short ypos, short color);
  void addOde(short xpos, short ypos);

  short getKilled();
  short getHurt();
  void kill();

private:

  short aCounter, cCounter, killed;
  short deathRoll, death, shake;

  short blocks[40][24];
  Color canvas[96][160];
  GLuint image;

  Anode* last;
  Anode* tAnode;

  list anodes;
  list cathodes;

};

#endif
