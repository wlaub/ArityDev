#include <cstdlib> 
#include <iostream>
#include <cmath>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "gun.h"
#include "event.h"

using namespace std;

void gun::initialize(short x, short y)
{
  xpos = x;
  ypos = y;

  bulletMax = 500;
  bullets = new bullet[bulletMax];

  for(x = 0; x < bulletMax; ++x)
  {
    bullets[x].x = 0;
    bullets[x].y = 0;
    bullets[x].xvel = 0;
    bullets[x].yvel = 0;
    bullets[x].active = 0;
  }

  off = 0;
  counter = 0;
  shotCounter = 0;
  fireRate = 20;
  fireVel = 3;
  damage = 1;
  radius = 3;
}

void gun::setColor(short r, short g, short b)
{

  short rbuff;
  short gbuff;
  short bbuff;
  short x;

  color[2].r = r;
  color[2].g = g;
  color[2].b = b;

  for(x = 0; x < 2; ++x)
  {
    rbuff = (short)(float(x)/2 * (float)r + float(2-x)/2 * 255);
    gbuff = (short)(float(x)/2 * (float)g + float(2-x)/2 * 255);
    bbuff = (short)(float(x)/2 * (float)b + float(2-x)/2 * 255);
    color[x].r = rbuff;
    color[x].g = gbuff;
    color[x].b = bbuff;
    rbuff = (short)(float(x)/2 * (float)r);
    gbuff = (short)(float(x)/2 * (float)g);
    bbuff = (short)(float(x)/2 * (float)b);
    color[4-x].r = rbuff;
    color[4-x].g = gbuff;
    color[4-x].b = bbuff;
  }


}

void gun::setBulletColor(short r, short g, short b)
{
  bulletColor.r = r;
  bulletColor.g = g;
  bulletColor.b = b;
}

void gun::setFire(short Rate, float Vel)
{
  fireRate = Rate;
  fireVel = Vel;
}

void gun::setBullets(short max)
{
  delete[] bullets;
  bulletMax = max;
  bullets = new bullet[max];
}

void gun::setDamage(short newD)
{
  damage = newD;
}
void gun::setRadius(float newR)
{
  radius = newR;
}
void gun::setPos(short x, short y)
{
  xpos = x;
  ypos = y;
}

void gun::neuterNum(short Num)
{
  if(Num == 0)
    if(bulletColor.r < bulletColor.g || bulletColor.r < bulletColor.b)
      return;
  if(Num == 1)
    if(bulletColor.g < bulletColor.r || bulletColor.g < bulletColor.b)
      return;
  if(Num == 2)
    if(bulletColor.b < bulletColor.g || bulletColor.b < bulletColor.r)
      return;
  off = 1;
  setColor(128,128,128);
  counter = 2;
}

void gun::neuter()
{
  off = 1;
  setColor(128,128,128);
  counter = 2;
}

bullet* gun::getBullets()
{
  return bullets;
}
short gun::getNum()
{
  return bulletMax;
}


void gun::clearBullets()
{
  for(short x = 0; x < bulletMax; ++x)
    bullets[x].active = 0;
}

void gun::drawGun()
{

  short i;
  short x;
  short y;
  short count;

  count = counter/2;
  glColor3ub(color[count].r, color[count].g, color[count].b);
  glBegin(GL_POINTS);
    glVertex2d(xpos, 720-ypos);
  glEnd();
  for(i = 1; i <= count; ++i)
  {
    glColor3ub(color[count-i].r, color[count-i].g, color[count-i].b);
    glBegin(GL_QUADS);
      glVertex2d(xpos-i, 720-ypos+i);
      glVertex2d(xpos+i, 720-ypos+i);
      glVertex2d(xpos+i, 720-ypos-i);
      glVertex2d(xpos-i, 720-ypos-i);
    glEnd();
  }
  for(i = count+1; i <5; ++i)
  {
    glColor3ub(color[i-count].r, color[i-count].g, color[i-count].b);
    glBegin(GL_QUADS);
      glVertex2d(xpos-i, 720-ypos+i);
      glVertex2d(xpos+i, 720-ypos+i);
      glVertex2d(xpos+i, 720-ypos-i);
      glVertex2d(xpos-i, 720-ypos-i);
    glEnd();
  }

}

void gun::drawBullets()
{
  if(off)
    return;

  short x, i;

  for(x = 0; x < bulletMax; ++x)
  {
    if(bullets[x].active == 1)
    {
      glColor3ub(bulletColor.r, bulletColor.g, bulletColor.b);
      glBegin(GL_TRIANGLE_FAN);
        glVertex2d(bullets[x].x, 720-bullets[x].y);
        for(i = 0; i <= 32; ++i)
          glVertex2d(bullets[x].x+radius*cos(32*6.28/i), 720-bullets[x].y-radius*sin(32*6.28/i));
      glEnd();
    }
  }

}

short gun::update()
{
  if(off)
    return 0;

  ++counter;
  if(counter > 10)
    counter = 0;
  return 0;
}

short gun::updateBullets(double playerX, double playerY)
{

  if(off)
    return 0;

  short x;
  short i;
  short j;
  short fired = 0;
  double magn;

  playerX*=3;
  playerY*=3;

  ++shotCounter;
  if(shotCounter > fireRate)
    shotCounter = 0;

  for(x = 0; x < bulletMax; ++x)
  {
    if(bullets[x].active == 1)
    {
      bullets[x].x += bullets[x].xvel;
      bullets[x].y += bullets[x].yvel;
      if(bullets[x].x < 0 || bullets[x].x >= 960 || bullets[x].y < 0 || bullets[x].y >= 720)
      {
        bullets[x].active = 0;
      }
    }
    else if(fired == 0 && shotCounter == 0)
    {
      fired = 1;
      bullets[x].x = xpos;
      bullets[x].y = ypos;
      magn = sqrt((xpos - playerX)*(xpos - playerX) + (ypos - playerY)*(ypos - playerY));
      bullets[x].xvel = fireVel*(double)(playerX - xpos)/magn;
      bullets[x].yvel = fireVel*(double)(playerY - ypos)/magn;
      bullets[x].active = 1;
    }
  }

  return fired;

}

short gun::checkCollision(double playerX, double playerY)
{

  if(off)
    return 0;

  short x;
  double testX;
  double testY;
  double dist;
  short hit = 0;

  for(x = 0; x < bulletMax; ++x)
  {
    if(bullets[x].active == 1)
    {
      testX = bullets[x].x/3;
      testY = bullets[x].y/3;
      dist = (testX-playerX)*(testX-playerX) + (testY-playerY)*(testY-playerY);
      if(dist < radius)
      {
        bullets[x].active = 0;
        gameEvents.addEvent(PLAYERHURT);
        hit = damage;
      }
    }
  }

  return hit;

}

short gun::checkPos(short x, short y)
{
  if(abs(x-xpos) < 6 && abs(y-ypos) < 6)
    return 1;
  return 0;
}

void gun::save(ofstream* fout)
{

  fout->write((char*)(&xpos), 2);
  fout->write((char*)(&ypos), 2);

  fout->write((char*)(&bulletMax), 2);

  fout->put(color[2].r);
  fout->put(color[2].g);
  fout->put(color[2].b);

  fout->put(bulletColor.r);
  fout->put(bulletColor.g);
  fout->put(bulletColor.b);

  fout->put(fireRate);

  fout->write((char*)(&fireVel), 4);
  fout->write((char*)(&radius), 4);
  fout->put(damage);

}

void gun::limit()
{
  short x;
  for(x = 0; x < bulletMax; ++x)
  {
    if(bullets[x].active == 1)
    {
      if(abs(bullets[x].x-xpos) > 25 || abs(bullets[x].y-ypos) > 50)
        bullets[x].active = 0;
    }
  }
}

void gun::adjust(short x, short y)
{
    switch(x/16)
    {
    case 0:
      if(y/16 == 0)
      {
        fireRate+=1;
      }
      else
      {
        fireVel-=.1;
      }
    break;
    case 1:
      if(y/16 == 0 && fireRate > 0)
      {
        fireRate-=1;
      }
      else
      {
        fireVel+=.1;
      }
    break;
    case 2:
      if(y/16 == 0)
      {cout << "X" << endl;
        radius+=.1;
      }
      else if(radius > 0)
      {cout << "Y" << endl;
        radius-=.1;
      }
    break;
    }

}

void gun::copy(gun* to)
{
  to->initialize(0,0);
  to->setFire(fireRate, fireVel);
  to->setRadius(radius);
  to->setBullets(bulletMax);
  to->setDamage(damage);
  to->setColor(color[2].r, color[2].g, color[2].b);
  to->setBulletColor(bulletColor.r, bulletColor.g, bulletColor.b);
}






