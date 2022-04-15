#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <cmath>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "purist.h"
#include "room.h"
#include "stuff.h"

using namespace std;

void stuff::initialize()
{
  short x;
  ifstream fin;
  fin.open("data/powerups.stf");
  short ub, lb;

  for(x = 0; x < 15; ++x)
  {
    jump1[x][0] = fin.get();
    jump1[x][1] = fin.get();
    jump1[x][2] = fin.get();
    fin.read((char*)(&jump1[x][3]), 2);
    fin.read((char*)(&jump1[x][4]), 2);
  }
  for(x = 0; x < 15; ++x)
  {
    jump2[x][0] = fin.get();
    jump2[x][1] = fin.get();
    jump2[x][2] = fin.get();
    fin.read((char*)(&jump2[x][3]), 2);
    fin.read((char*)(&jump2[x][4]), 2);
  }
  for(x = 0; x < 30; ++x)
  {
    health[x][0] = fin.get();
    health[x][1] = fin.get();
    health[x][2] = fin.get();
    fin.read((char*)(&health[x][3]), 2);
    fin.read((char*)(&health[x][4]), 2);
  }
  for(x = 0; x < 3; ++x)
  {
    all[x][0] = fin.get();
    all[x][1] = fin.get();
    all[x][2] = fin.get();
    fin.read((char*)(&all[x][3]), 2);
    fin.read((char*)(&all[x][4]), 2);
  }

  fin.close();

  for(x = 0; x < 4; ++x)
    bosses[x] = 0;
}

void stuff::save()
{

  short x;
  ofstream fout;
  fout.open("data/powerups.stf");
  short ub, lb;

  for(x = 0; x < 15; ++x)
  {
    fout.put(jump1[x][0]);
    fout.put(jump1[x][1]);
    fout.put(jump1[x][2]);
    fout.write((char*)(&jump1[x][3]), 2);
    fout.write((char*)(&jump1[x][4]), 2);
  }
  for(x = 0; x < 15; ++x)
  {
    fout.put(jump2[x][0]);
    fout.put(jump2[x][1]);
    fout.put(jump2[x][2]);
    fout.write((char*)(&jump2[x][3]), 2);
    fout.write((char*)(&jump2[x][4]), 2);
  }
  for(x = 0; x < 30; ++x)
  {
    fout.put(health[x][0]);
    fout.put(health[x][1]);
    fout.put(health[x][2]);
    fout.write((char*)(&health[x][3]), 2);
    fout.write((char*)(&health[x][4]), 2);
  }
  for(x = 0; x < 3; ++x)
  {
    fout.put(all[x][0]);
    fout.put(all[x][1]);
    fout.put(all[x][2]);
    fout.write((char*)(&all[x][3]), 2);
    fout.write((char*)(&all[x][4]), 2);
  }

  fout.close();

}

void stuff::clear()
{
  short x;
  for(x = 0; x < 15; ++x)
  {
    jump1[x][0] = 1;
    jump2[x][0] = 1;
    health[x][0] = 1;
    health[x+15][0] = 1;
  }
  for(x = 0; x < 3; ++x)
    all[x][0] = 1;
}

short stuff::checkPowerup(purist* dood, short roomX, short roomY)
{

  short x;
  short type = 0;

  for(x = 0; x < 15; ++ x)
  {
    if(jump1[x][0] != 0)
    {
      if(jump1[x][1] == roomX && jump1[x][2] == roomY)
      {
        if(dood->xpos >= jump1[x][3]/3 && dood->xpos <= jump1[x][3]/3+4)
          if(dood->ypos >= jump1[x][4]/3 && dood->ypos <= jump1[x][4]/3+4)
          {
            type = 1;
            jump1[x][0] = 0;
            dood->jump1-=.1;
          }
      }
    }
    if(jump2[x][0] != 0)
    {
      if(jump2[x][1] == roomX && jump2[x][2] == roomY)
      {
        if(dood->xpos >= jump2[x][3]/3 && dood->xpos <= jump2[x][3]/3+4)
          if(dood->ypos >= jump2[x][4]/3 && dood->ypos <= jump2[x][4]/3+4)
          {
            type = 2;
            jump2[x][0] = 0;
            dood->jump2-=.1;
          }
      }
    }
  }

  for(x = 0; x < 30; ++ x)
  {
    if(health[x][0] != 0)
    {
      if(health[x][1] == roomX && health[x][2] == roomY)
      {
        if(dood->xpos >= health[x][3]/3 && dood->xpos <= health[x][3]/3+4)
          if(dood->ypos >= health[x][4]/3 && dood->ypos <= health[x][4]/3+4)
          {
            type = 3;
            health[x][0] = 0;
            dood->health+=4;
            dood->maxHealth+=4;
          }
      }
    }
  }

  for(x = 0; x < 3; ++ x)
  {
    if(all[x][0] != 0)
    {
      if(all[x][1] == roomX && all[x][2] == roomY)
      {
        if(dood->xpos >= all[x][3]/3 && dood->xpos <= all[x][3]/3+4)
          if(dood->ypos >= all[x][4]/3 && dood->ypos <= all[x][4]/3+4)
          {
            type = 4;
            all[x][0] = 0;
            dood->health+=20;
            dood->maxHealth+=20;
            dood->jump1-=.4;
            dood->jump2-=.2;
          }
      }
    }
  }

  return type;

}

void stuff::forceDraw(short roomX, short roomY)
{

  short x;
  short i;
  short j;

  for(x = 0; x < 15; ++ x)
  {
    glColor4f(0,0,1,1);
    if(jump1[x][1] == roomX && jump1[x][2] == roomY)
    {
      glBegin(GL_QUADS);
        glVertex2d(jump1[x][3], 720-jump1[x][4]-12);
        glVertex2d(jump1[x][3], 720-jump1[x][4]);
        glVertex2d(jump1[x][3]+12, 720-jump1[x][4]);
        glVertex2d(jump1[x][3]+12, 720-jump1[x][4]-12);
      glEnd();
    }
    glColor4f(0,1,0,1);
    if(jump2[x][0] != 0)
    {
      glBegin(GL_QUADS);
        glVertex2d(jump2[x][3], 720-jump2[x][4]-12);
        glVertex2d(jump2[x][3], 720-jump2[x][4]);
        glVertex2d(jump2[x][3]+12, 720-jump2[x][4]);
        glVertex2d(jump2[x][3]+12, 720-jump2[x][4]-12);
      glEnd();
    }
  }

  glColor4f(1,0,0,1);
  for(x = 0; x < 30; ++ x)
  {
    if(health[x][1] == roomX && health[x][2] == roomY)
    {
      glBegin(GL_QUADS);
        glVertex2d(health[x][3], 720-health[x][4]-12);
        glVertex2d(health[x][3], 720-health[x][4]);
        glVertex2d(health[x][3]+12, 720-health[x][4]);
        glVertex2d(health[x][3]+12, 720-health[x][4]-12);
      glEnd();
    }
  }

  glColor4f(1,1,1,1);
  for(x = 0; x < 3; ++ x)
  {
    if(all[x][1] == roomX && all[x][2] == roomY)
    {
      glBegin(GL_QUADS);
        glVertex2d(all[x][3], 720-all[x][4]-12);
        glVertex2d(all[x][3], 720-all[x][4]);
        glVertex2d(all[x][3]+12, 720-all[x][4]);
        glVertex2d(all[x][3]+12, 720-all[x][4]-12);
      glEnd();
    }
  }
}

void stuff::drawPowerups(short roomX, short roomY)
{

  short x;
  short i;
  short j;

  for(x = 0; x < 15; ++ x)
  {
    glColor4f(0,0,1,1);
    if(jump1[x][0] != 0)
    {
      if(jump1[x][1] == roomX && jump1[x][2] == roomY)
      {
        glBegin(GL_QUADS);
          glVertex2d(jump1[x][3], 720-jump1[x][4]-12);
          glVertex2d(jump1[x][3], 720-jump1[x][4]);
          glVertex2d(jump1[x][3]+12, 720-jump1[x][4]);
          glVertex2d(jump1[x][3]+12, 720-jump1[x][4]-12);
        glEnd();
      }
    }
    glColor4f(0,1,0,1);
    if(jump2[x][0] != 0)
    {
      if(jump2[x][1] == roomX && jump2[x][2] == roomY)
      {
        glBegin(GL_QUADS);
          glVertex2d(jump2[x][3], 720-jump2[x][4]-12);
          glVertex2d(jump2[x][3], 720-jump2[x][4]);
          glVertex2d(jump2[x][3]+12, 720-jump2[x][4]);
          glVertex2d(jump2[x][3]+12, 720-jump2[x][4]-12);
        glEnd();
      }
    }
  }

  glColor4f(1,0,0,1);
  for(x = 0; x < 30; ++ x)
  {
    if(health[x][0] != 0)
    {
      if(health[x][1] == roomX && health[x][2] == roomY)
      {
        glBegin(GL_QUADS);
          glVertex2d(health[x][3], 720-health[x][4]-12);
          glVertex2d(health[x][3], 720-health[x][4]);
          glVertex2d(health[x][3]+12, 720-health[x][4]);
          glVertex2d(health[x][3]+12, 720-health[x][4]-12);
        glEnd();
      }
    }
  }

  glColor4f(1,1,1,1);
  for(x = 0; x < 3; ++ x)
  {
    if(all[x][0] != 0)
    {
      if(all[x][1] == roomX && all[x][2] == roomY)
      {
        glBegin(GL_QUADS);
          glVertex2d(all[x][3], 720-all[x][4]-12);
          glVertex2d(all[x][3], 720-all[x][4]);
          glVertex2d(all[x][3]+12, 720-all[x][4]);
          glVertex2d(all[x][3]+12, 720-all[x][4]-12);
        glEnd();
      }
    }
  }

}

void stuff::addPowerup(short type, short xpos, short ypos, short roomx, short roomy)
{
  short x, y;
  switch(type)
  {
  case 1:
    x = 0;
    while((health[x][1] || health[x][2] || health[x][3] || health[x][4]) && x < 30)
      ++x;
    if(x == 30)
      return;
    health[x][0] = 1;
    health[x][1] = roomx;
    health[x][2] = roomy;
    health[x][3] = xpos-6;
    health[x][4] = ypos-6;
  break;
  case 2:
    x = 0;
    while((jump2[x][1] || jump2[x][2] || jump2[x][3] || jump2[x][4]) && x < 15)
      ++x;
    if(x == 15)
      return;
    jump2[x][0] = 1;
    jump2[x][1] = roomx;
    jump2[x][2] = roomy;
    jump2[x][3] = xpos-6;
    jump2[x][4] = ypos-6;
  break;
  case 3:
    x = 0;
    while((jump1[x][1] || jump1[x][2] || jump1[x][3] || jump1[x][4]) && x < 15)
      ++x;
    if(x == 15)
      return;
    jump1[x][0] = 1;
    jump1[x][1] = roomx;
    jump1[x][2] = roomy;
    jump1[x][3] = xpos-6;
    jump1[x][4] = ypos-6;
  break;
  case 4:
    x = 0;
    while((all[x][1] || all[x][2] || all[x][3] || all[x][4]) && x < 3)
      ++x;
    if(x == 3)
      return;
    all[x][0] = 1;
    all[x][1] = roomx;
    all[x][2] = roomy;
    all[x][3] = xpos-6;
    all[x][4] = ypos-6;
  break;
  }
}

void stuff::erase(short roomx, short roomy, short xpos, short ypos)
{
  short x;
  xpos-=6;
  ypos-=6;
  for(x = 0; x < 15; ++ x)
  {
    if(jump1[x][0] != 0)
    {
      if(jump1[x][1] == roomx && jump1[x][2] == roomy)
      {
        if(abs(xpos-jump1[x][3]) <= 6)
          if(abs(ypos-jump1[x][4]) <= 6)
          {
            jump1[x][0] = 1;
            jump1[x][1] = 0;
            jump1[x][2] = 0;
            jump1[x][3] = 0;
            jump1[x][4] = 0;
          }
      }
    }
    if(jump2[x][0] != 0)
    {
      if(jump2[x][1] == roomx && jump2[x][2] == roomy)
      {
        if(abs(xpos-jump2[x][3]) <= 6)
          if(abs(ypos-jump2[x][4]) <= 6)
          {
            jump2[x][0] = 1;
            jump2[x][1] = 0;
            jump2[x][2] = 0;
            jump2[x][3] = 0;
            jump2[x][4] = 0;
          }
      }
    }
  }

  for(x = 0; x < 30; ++ x)
  {
    if(health[x][0] != 0)
    {
      if(health[x][1] == roomx && health[x][2] == roomy)
      {
        if(abs(xpos-health[x][3]) <= 6)
          if(abs(ypos-health[x][4]) <= 6)
          {
            health[x][0] = 1;
            health[x][1] = 0;
            health[x][2] = 0;
            health[x][3] = 0;
            health[x][4] = 0;
          }
      }
    }
  } 

  for(x = 0; x < 3; ++ x)
  {
    if(all[x][0] != 0)
    {
      if(all[x][1] == roomx && all[x][2] == roomy)
      {
        if(abs(xpos-all[x][3]) <= 6)
          if(abs(ypos-all[x][4]) <= 6)
          {
            all[x][0] = 1;
            all[x][1] = 0;
            all[x][2] = 0;
            all[x][3] = 0;
            all[x][4] = 0;
          }
      }
    }
  } 

}
