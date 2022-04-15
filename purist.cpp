#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <cmath>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "event.h"
#include "purist.h"

using namespace std;

void purist::initialize()
{
  xpos = 160;
  ypos = 231;
  xvel = 0;
  yvel = 0;
  jumps = 0;
  onGround = 0;
  jump1 = -2;
  jump2 = -1.5;
  maxHealth = 60;
  health = 60;

  charged = 000;
  hitCounter = 0;

  locked = 0;

  roomX = 7;
  roomY = 6;

  for(short x = 0; x < 15; ++x)
    for(short y = 0; y < 13; ++y)
      visited[x][y] = 0;

  visited[7][6] = 2;

  memset(bosses, 0, 8);

  trail.initialize();
}

void purist::addPoint()
{
  if(!charged)
  {
    clearPath();
    return;
  }
  tPoint = new point;
  tPoint->x = xpos;
  tPoint->y = ypos;
  trail.add(tPoint);
  --charged;
}

void purist::draw()
{

  short i;
  short detail = 16;
  double tempX, tempY;
  double tempXVel, tempYVel;
  short dir;
  short number = 0;

  tempX = xpos;
  tempY = ypos;
  tempXVel = xvel/detail;
  tempYVel = yvel/detail;
  if(onGround)
  {
    tempYVel = 0;
    tempY = ypos;
  }


  if(yvel < 0 && yvel > -.4 && jumps == 2 && charged)
  {
    double factor = abs(yvel)/.05;
    glColor4f(1,1,1,1-abs(yvel)/.4);
    glBegin(GL_QUADS);
      glVertex2d(3*xpos+2+factor, 720-3*ypos+1+factor);
      glVertex2d(3*xpos+2+factor, 720-3*ypos-2-factor);
      glVertex2d(3*xpos-1-factor, 720-3*ypos-2-factor);
      glVertex2d(3*xpos-1-factor, 720-3*ypos+1+factor);
    glEnd();
    glClear(GL_DEPTH_BUFFER_BIT);
  }

  glColor3f(1,1,1);
  glBegin(GL_QUADS);
    glVertex2d(3*xpos+2, 720-3*ypos+1);
    glVertex2d(3*xpos+2, 720-3*ypos-2);
    glVertex2d(3*xpos-1, 720-3*ypos-2);
    glVertex2d(3*xpos-1, 720-3*ypos+1);
  glEnd();

  for(i = detail; i > 0; --i)
  {
    glColor3f(i/(float)detail,i/(float)detail,i/(float)detail);
    glBegin(GL_QUADS);
      glVertex2d(3*tempX+2, 720-3*tempY+1);
      glVertex2d(3*tempX+2, 720-3*tempY-2);
      glVertex2d(3*tempX-1, 720-3*tempY-2);
      glVertex2d(3*tempX-1, 720-3*tempY+1);
    glEnd();
    tempX-=tempXVel;
    tempY-=tempYVel;
  }

  if(charged)
  {
    if(hitCounter)
    {
      drawLightning();
      return;
    }
    trail.setPointer(1);
    if(trail.getLength())
    {
      tempXVel = ((point*)(trail.getPointer()))->x;
      tempYVel = ((point*)(trail.getPointer()))->y;
      while(!trail.decPointer())
      {
        tempX = ((point*)(trail.getPointer()))->x;
        tempY = ((point*)(trail.getPointer()))->y;
        dir = 1;
        if(tempX < tempXVel)
          dir = -1;
        glColor4f(1,1,1,.75*(float)(charged-number)/charged);
        glBegin(GL_QUADS);
          glVertex2d(3*tempX+dir, 720-3*tempY-2);
          glVertex2d(3*tempX+dir, 720-3*tempY+1);
          glVertex2d(3*tempXVel-dir, 720-3*tempY+1);
          glVertex2d(3*tempXVel-dir, 720-3*tempY-2);
          glVertex2d(3*tempXVel-1, 720-3*tempY-2);
          glVertex2d(3*tempXVel-1, 720-3*tempYVel+1);
          glVertex2d(3*tempXVel+2, 720-3*tempYVel+1);
          glVertex2d(3*tempXVel+2, 720-3*tempY-2);
        glEnd();
        tempXVel = tempX;
        tempYVel = tempY;
        ++number;
      }
    }
  }
}

void purist::drawLightning()
{
  double tempX, tempY;
  double tempXVel, tempYVel;
  short dir;
  short number = 0;
  trail.setPointer(1);
  if(trail.getLength())
  {
    tempXVel = ((point*)(trail.getPointer()))->x;
    tempYVel = ((point*)(trail.getPointer()))->y;
    while(!trail.decPointer())
    {
      tempX = ((point*)(trail.getPointer()))->x;
      tempY = ((point*)(trail.getPointer()))->y;
      dir = 1;
      if(tempX < tempXVel)
        dir = -1;
      glColor4f(1,1,rand()/(float)RAND_MAX,1);
      glBegin(GL_QUADS);
        glVertex2d(3*tempX+dir+rand()/(float)RAND_MAX, 720-3*tempY-2+rand()/(float)RAND_MAX);
        glVertex2d(3*tempX+dir+rand()/(float)RAND_MAX, 720-3*tempY+1+rand()/(float)RAND_MAX);
        glVertex2d(3*tempXVel-dir+rand()/(float)RAND_MAX, 720-3*tempY+1+rand()/(float)RAND_MAX);
        glVertex2d(3*tempXVel-dir+rand()/(float)RAND_MAX, 720-3*tempY-2+rand()/(float)RAND_MAX);
        glVertex2d(3*tempXVel-1+rand()/(float)RAND_MAX, 720-3*tempY-2+rand()/(float)RAND_MAX);
        glVertex2d(3*tempXVel-1+rand()/(float)RAND_MAX, 720-3*tempYVel+1+rand()/(float)RAND_MAX);
        glVertex2d(3*tempXVel+2+rand()/(float)RAND_MAX, 720-3*tempYVel+1+rand()/(float)RAND_MAX);
        glVertex2d(3*tempXVel+2+rand()/(float)RAND_MAX, 720-3*tempY-2+rand()/(float)RAND_MAX);
      glEnd();
      tempXVel = tempX;
      tempYVel = tempY;
      ++number;
    }
  }
}

void purist::clearPath()
{
  trail.setPointer(0);
  while(trail.getLength())
    trail.delPointer();
}

void purist::load()
{
  short x;

  ifstream fin;
  fin.open("profile", ios_base::binary);

  initialize();

  roomX = fin.get();
  roomY = fin.get();
  xpos = 4*fin.get() + 2;
  ypos = 4*fin.get() + 2;
  maxHealth = fin.get();
  jump1 = -1*(double)fin.get()/10;
  jump2 = -1*(double)fin.get()/10;
  health = maxHealth;

  for(short x = 0; x < 15; ++x)
    for(short y = 0; y < 13; ++y)
      visited[x][y] = fin.get();

  for(x = 0; x < 4; ++x)
    bosses[x] = fin.get();


  fin.close();
}

void purist::save(short roomX, short roomY)
{

  short x, y;

  ofstream fout;
  fout.open("profile", ios_base::binary);
  fout.put(roomX);
  fout.put(roomY);
  fout.put((short)(xpos/4));
  fout.put((short)(ypos/4));
  fout.put(maxHealth);
  fout.put((short)(-1*jump1*10));
  fout.put((short)(-1*jump2*10));
  health = maxHealth;

  for(short x = 0; x < 15; ++x)
    for(short y = 0; y < 13; ++y)
      fout.put(visited[x][y]);

  for(x = 0; x < 4; ++x)
    fout.put((char)(bosses[x]));

  fout.close();

}

