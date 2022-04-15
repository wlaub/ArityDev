#include <cstdlib>
#include <iostream>
#include <cmath>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "boss.h"
#include "event.h"

void boss::init()
{

  aCounter = 0;
  cCounter = 0;
  killed = 0;
  deathRoll = 0;
  death = 0;

  anodes.initialize();
  cathodes.initialize();

  memset(blocks, 0, 1920);

  glGenTextures(1, &image);
  glBindTexture(GL_TEXTURE_2D, image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  refresh();

}

void boss::refresh()
{
  short x, y, i, j;

  Color black;
  Color colors[4];

  memset(canvas, 0, 61440);
  memset(colors, 0, 16);
  black.r = 0; black.g = 0; black.b = 0; black.a = 255;
  colors[0].r = 255; colors[0].a = 255;
  colors[1].g = 255; colors[1].a = 255;
  colors[2].b = 255; colors[2].a = 255;
  colors[3].r = 255; colors[3].g = 255; colors[3].b = 255; colors[3].a = 255;

  for(x = 0; x < 40; ++x)
    for(y = 0; y < 24; ++y)
    {
      if(blocks[x][y])
      {
        for(i = 0; i < 4; ++i)
          for(j = 0; j < 4; ++j)
            canvas[4*y+i][4*x+j] = black;

        if(x > 0)
        {
          if(y < 23 && !blocks[x-1][y+1])
            canvas[4*y+3][4*x] = colors[blocks[x][y]-1];
          if(y > 0 && !blocks[x-1][y-1])
            canvas[4*y][4*x] = colors[blocks[x][y]-1];
        }
        if(x == 0 || (x > 0 && !blocks[x-1][y]))
          for(i = 0; i < 4; ++i)
            canvas[4*y+i][4*x] = colors[blocks[x][y]-1];

        if(y == 0 || (y > 0 && !blocks[x][y-1]))
          for(i = 0; i < 4; ++i)
            canvas[4*y][4*x+i] = colors[blocks[x][y]-1];
        if(y == 23 || (y < 23 && !blocks[x][y+1]))
          for(i = 0; i < 4; ++i)
            canvas[4*y+3][4*x+i] = colors[blocks[x][y]-1];
        if(x < 39)
        {
          if(y < 23 && !blocks[x+1][y+1])
            canvas[4*y+3][4*x+3] = colors[blocks[x][y]-1];
          if(y > 0 && !blocks[x+1][y-1])
            canvas[4*y][4*x+3] = colors[blocks[x][y]-1];
        }
        if(x == 39 || (x < 39 && !blocks[x+1][y]))
          for(i = 0; i < 4; ++i)
            canvas[4*y+i][4*x+3] = colors[blocks[x][y]-1];

      }
    }
    glBindTexture(GL_TEXTURE_2D, image);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 160, 96, 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas);
}

void boss::load(ifstream* fin)
{

  short x, y;
  short anodeNum;

  for(x = 0; x < 40; ++x)
    for(y = 0; y < 24; ++y)
      blocks[x][y] = fin->get();

  refresh();

  anodeNum = fin->get();
  for(x = 0; x < anodeNum; ++x)
  {
    tAnode = new Anode;
    fin->read((char*)(&(tAnode->x)), 4);
    fin->read((char*)(&(tAnode->y)), 4);
    fin->read((char*)(&(tAnode->amount)), 2);
    tAnode->killed = 0;
    anodes.add(tAnode);
  }
  for(x = 0; x < anodeNum; ++x)
  {
    tAnode = new Anode;
    fin->read((char*)(&(tAnode->x)), 4);
    fin->read((char*)(&(tAnode->y)), 4);
    fin->read((char*)(&(tAnode->amount)), 2);
    tAnode->killed = 0;
    cathodes.add(tAnode);
  }

}
void boss::save(ofstream* fout)
{

  short x, y;

  for(x = 0; x < 40; ++x)
    for(y = 0; y < 24; ++y)
      fout->put((unsigned char)(blocks[x][y]));

  fout->put(anodes.getLength());
  anodes.setPointer(0);
  if(anodes.getLength()) do
  {
    tAnode = (Anode*)anodes.getPointer();
    fout->write((char*)(&(tAnode->x)), 4);
    fout->write((char*)(&(tAnode->y)), 4);
    fout->write((char*)(&(tAnode->amount)), 2);
  } while(!anodes.incPointer());
  cathodes.setPointer(0);
  if(cathodes.getLength()) do
  {
    tAnode = (Anode*)cathodes.getPointer();
    fout->write((char*)(&(tAnode->x)), 4);
    fout->write((char*)(&(tAnode->y)), 4);
    fout->write((char*)(&(tAnode->amount)), 2);
  } while(!cathodes.incPointer());
}

void boss::reset()
{
  killed = 0;
  anodes.setPointer(0);
  if(anodes.getLength()) do
  {
    tAnode = (Anode*)(anodes.getPointer());
    tAnode->killed = 0;
  } while(!anodes.incPointer());

  cathodes.setPointer(0);
  if(cathodes.getLength()) do
  {
    tAnode = (Anode*)(cathodes.getPointer());
    tAnode->killed = 0;
  } while(!cathodes.incPointer());

}

short boss::checkBlock(short xpos, short ypos)
{
  if(killed || shake)
    return 0;
  short xAdjust, yAdjust;
  xAdjust = (xpos)/4-20;
  yAdjust = (ypos)/4-1;
  if(xAdjust < 0 || xAdjust > 39 || yAdjust < 0 || yAdjust > 23)
    return 0;
  return blocks[xAdjust][yAdjust];
}

void boss::collide(purist* dood)
{
  if(killed == 2)
    return;

  if(!dood->hitCounter)
    dood->addPoint();
  else
  {
    --dood->hitCounter;
    if(!dood->hitCounter)
    {
      dood->charged = 0;
      dood->clearPath();
    }
  }

  shake = 0;
  if(death || dood->hitCounter)
    shake = 1;

  killed = 1;
  anodes.setPointer(0);
  if(anodes.getLength()) do
  {
    tAnode = (Anode*)(anodes.getPointer());
    if(tAnode->killed == 1 && !dood->hitCounter)
      tAnode->killed = 2;
    if(!tAnode->killed && !dood->hitCounter)
    {
      if(abs(3*dood->xpos - tAnode->x) < 5 && abs(3*dood->ypos - tAnode->y) < 5)
      {
        if(!dood->charged)
          gameEvents.addEvent(PLAYERCHARGE);
        dood->charged = tAnode->amount;
        last = tAnode;
      }
    }
  } while(!anodes.incPointer());

  cathodes.setPointer(0);
  if(cathodes.getLength()) do
  {
    tAnode = (Anode*)(cathodes.getPointer());
    if(tAnode->killed == 1 && !dood->hitCounter)
      tAnode->killed = 2;
    if(!tAnode->killed)
    {
      killed = 0;
      if(abs(3*dood->xpos - tAnode->x) < 5 && abs(3*dood->ypos - tAnode->y) < 5 && dood->charged && !dood->hitCounter)
      {
        last->killed = 1;
        tAnode->killed = 1;
        dood->hitCounter = 50;
        gameEvents.addEvent(BOSSHURT);
      }
    }
  } while(!cathodes.incPointer());

  if(killed && !dood->hitCounter && !death)
  {
    deathRoll = 300;
    death = 1;
  }
  if(death && deathRoll == 0)
  {
    killed = 2;
    gameEvents.addEvent(BOSSDEAD);
  }

  if(death)
  {
    --deathRoll;
    gameEvents.addEvent(DEATHROLL);
  }

}

void boss::draw()
{
  if(killed == 2)
    return;

  glColor4f(1,1,1,.75);
  glBegin(GL_QUADS);
    glVertex2d(0,720);
    glVertex2d(3,720);
    glVertex2d(3,0);
    glVertex2d(0,0);
    glVertex2d(960,720);
    glVertex2d(957,720);
    glVertex2d(957,0);
    glVertex2d(960,0);
    glVertex2d(0,0);
    glVertex2d(0,3);
    glVertex2d(960,3);
    glVertex2d(960,0);
    glVertex2d(0,720);
    glVertex2d(0,717);
    glVertex2d(960,717);
    glVertex2d(960,720);
  glEnd();
  glColor4f(1,1,1,.5);
  glBegin(GL_QUADS);
    glVertex2d(3,720);
    glVertex2d(6,720);
    glVertex2d(6,0);
    glVertex2d(3,0);
    glVertex2d(957,720);
    glVertex2d(954,720);
    glVertex2d(954,0);
    glVertex2d(957,0);
    glVertex2d(3,3);
    glVertex2d(3,6);
    glVertex2d(960,6);
    glVertex2d(960,3);
    glVertex2d(3,717);
    glVertex2d(3,714);
    glVertex2d(960,714);
    glVertex2d(960,717);
  glEnd();
  glColor4f(1,1,1,.25);
  glBegin(GL_QUADS);
    glVertex2d(6,720);
    glVertex2d(9,720);
    glVertex2d(9,0);
    glVertex2d(6,0);
    glVertex2d(954,720);
    glVertex2d(951,720);
    glVertex2d(951,0);
    glVertex2d(954,0);
    glVertex2d(3,6);
    glVertex2d(3,9);
    glVertex2d(960,9);
    glVertex2d(960,6);
    glVertex2d(3,714);
    glVertex2d(3,711);
    glVertex2d(960,711);
    glVertex2d(960,714);
  glEnd();

  glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image);
    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
      glTexCoord2d(0, 0); glVertex2d(240+(rand()*shake)/(float)RAND_MAX, 720-12+(rand()*shake)/(float)RAND_MAX);
      glTexCoord2d(1, 0); glVertex2d(720+(rand()*shake)/(float)RAND_MAX, 720-12+(rand()*shake)/(float)RAND_MAX);
      glTexCoord2d(1, 1); glVertex2d(720+(rand()*shake)/(float)RAND_MAX, 720-300+(rand()*shake)/(float)RAND_MAX);
      glTexCoord2d(0, 1); glVertex2d(240+(rand()*shake)/(float)RAND_MAX, 720-300+(rand()*shake)/(float)RAND_MAX);
    glEnd();
  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_TEXTURE_2D);

  ++aCounter;
  aCounter%=32;
  glColor3f(.5+aCounter/62.0,.5+aCounter/62.0,.5+aCounter/62.0);
  
  anodes.setPointer(0);
  if(anodes.getLength()) do
  {
    tAnode = (Anode*)(anodes.getPointer());
    if(tAnode->killed != 2)
    {
      glBegin(GL_QUADS);
        glVertex2d(tAnode->x+4, 720-tAnode->y+4);
        glVertex2d(tAnode->x-4, 720-tAnode->y+4);
        glVertex2d(tAnode->x-4, 720-tAnode->y-4);
        glVertex2d(tAnode->x+4, 720-tAnode->y-4);
      glEnd();
    }
  } while(!anodes.incPointer());

  cCounter = 31-aCounter;
  glColor3f(.5+cCounter/62.0,.5+cCounter/62.0,.5+cCounter/62.0);

  cathodes.setPointer(0);
  if(cathodes.getLength()) do
  {
    tAnode = (Anode*)(cathodes.getPointer());
    if(tAnode->killed != 2)
    {
      glBegin(GL_QUADS);
        glVertex2d(tAnode->x+4, 720-tAnode->y+4);
        glVertex2d(tAnode->x-4, 720-tAnode->y+4);
        glVertex2d(tAnode->x-4, 720-tAnode->y-4);
        glVertex2d(tAnode->x+4, 720-tAnode->y-4);
      glEnd();
    }
  } while(!cathodes.incPointer());

}

void boss::drawBlock(short xpos, short ypos, short color)
{
  if(xpos < 0 || xpos > 39 || ypos < 0 || ypos > 23 || color < 0 || color > 4 || color == blocks[xpos][ypos])
    return;
  blocks[xpos][ypos] = color;
  refresh();
}
void boss::addOde(short xpos, short ypos)
{
  tAnode = new Anode;
  tAnode->x = xpos;
  tAnode->y = ypos;
  tAnode->killed = 0;
  tAnode->amount = 1000;

  if(anodes.getLength() > cathodes.getLength())
    cathodes.add(tAnode);
  else
    anodes.add(tAnode);
}

short boss::getKilled()
{
  return killed;
}
short boss::getHurt()
{
  return shake;
}

void boss::kill()
{
  killed = 2;
}






