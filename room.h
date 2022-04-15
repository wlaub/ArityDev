#ifndef ROOM_H
#define ROOM_H

#include "purist.h"
#include "stuff.h"
#include "gun.h"
#include "boss.h"

class room
{
public:
  room();

  void initialize();
  void reset();

  void setBosses(purist* dood, short enable = 0);
  void refresh();

  void load(short xpos, short ypos);

  void checkPlatforms(short xpos, short ypos);

  void save(short xpos, short ypos);

  short playerSave(purist* dood, stuff* powerups);
  short warp(purist* dood);

  void drawBlock(short x, short y, Color* targ, char blocks, Color color);
  void draw();
  void drawMap(short x, short y);

  void update();

  short collide(purist* dood);
  char getBlock(short x, short y);
  char getBlockTarg(short x, short y, char targ);
  char getBlockFore(short x, short y);
  char getBlockBack1(short x, short y);
  char getBlockBack2(short x, short y);
  short getX();
  short getY();
  short hasABoss();

  void set(short x, short y, short layer, char color);

  void initPlatforms();
  void updatePlatforms();
  void drawPlatforms();
  void clearPlatforms();

  void addPlatforms(short newColor);
  void adjustPlatforms(short xpos, short ypos);

  void addSave(short xpos, short ypos);
  void erase(short xpos, short ypos);

  void addTile(short type, short layer, short xpos, short ypos);
  void addBossTile(short type, short xpos, short ypos);
  void addBossOde(short xpos, short ypos);
  void addGun(short xpos, short ypos, gun *nGun);

private:
  Color fore[240][320];
  Color miniData[30][40];

  GLuint image;
  GLuint miniMap;

  char foreArray[40][60];
  char backArray1[40][60];
  char backArray2[40][60];

  short roomX;
  short roomY;

  short platNum;
  
  double platCenter;
  short platDir;
  double platX[15];
  double platY[15];
  Color color;
  float velocity;
  float size;
  float maxDist;

  short saveNum;
  short saveX;
  short saveY;
  short savePhase;

  short bosses[4];

  Color red[3];
  Color blue[3];
  Color green[3];
  Color white[3];
  Color gray[3];

  short edgeCounter;

  list gunz;
  gun *tempGun;

  short hasBoss;
  boss Boss;

};

#endif
