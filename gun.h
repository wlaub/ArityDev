#ifndef GUN_H
#define GUN_H

#include <fstream>
#include "structs.h"

class gun
{
public:
  void initialize(short x, short y);
  void setColor(short r, short g, short b);
  void setBulletColor(short r, short g, short b);
  void setFire(short Rate, float Vel);
  void setBullets(short max);
  void setDamage(short newD);
  void setRadius(float newR);
  void setPos(short x, short y);
  void neuterNum(short Num);
  void neuter();

  bullet* getBullets();
  short getNum();

  void clearBullets();

  void drawGun();
  void drawBullets();
  short update();
  short updateBullets(double playerX, double playerY);
  short checkCollision(double playerX, double playerY);

  short checkPos(short x, short y);

  void save(std::ofstream* fout);

  void limit();
  void adjust(short x, short y);

  void copy(gun* to);

private:
  bullet* bullets;
  short xpos;
  short ypos;
  short counter;
  short shotCounter;
  short fireRate;
  float fireVel;
  float radius;
  short bulletMax;
  short damage;
  Color color[5];
  Color bulletColor;
  short off;
};

#endif
