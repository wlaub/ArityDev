#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <cmath>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "event.h"
#include "room.h"

using namespace std;

  room::room()
  {
    initialize();
  }

  void room::initialize()
  {

    memset(foreArray, 0, 2400);
    memset(backArray1, 0, 2400);
    memset(backArray2, 0, 2400);

    memset(miniData, 0, 4800);
    for(short x = 0; x < 40; ++x)
      for(short y = 0; y < 30; ++y)
      {
        miniData[y][x].a = 255;
      }

    gunz.initialize();

    savePhase = 0;
 
    for(short x = 0; x < 4; ++x)
      bosses[x] = 0;

    hasBoss = 0;

    velocity = .2;
    size = 4;
    maxDist = 24;

    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_2D, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &miniMap);
    glBindTexture(GL_TEXTURE_2D, miniMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    white[0].r = 255; white[0].g = 255; white[0].b = 255; white[0].a = 255;
    white[1].r = 128; white[1].g = 128; white[1].b = 128; white[1].a = 255;
    white[2].r = 64;  white[2].g = 64;  white[2].b = 64;  white[2].a = 255;

    gray[0].r = 128; gray[0].g = 128; gray[0].b = 128; gray[0].a = 255;
    gray[1].r = 64;  gray[1].g = 64;  gray[1].b = 64;  gray[1].a = 255;
    gray[2].r = 32;  gray[2].g = 32;  gray[2].b = 32;  gray[2].a = 255;

    red[0].r = 255; red[0].g = 0; red[0].b = 0; red[0].a = 255;
    red[1].r = 128; red[1].g = 0; red[1].b = 0; red[1].a = 255;
    red[2].r = 96;  red[2].g = 0; red[2].b = 0; red[2].a = 255;

    green[0].r = 0; green[0].g = 255; green[0].b = 0; green[0].a = 255;
    green[1].r = 0; green[1].g = 128; green[1].b = 0; green[1].a = 255;
    green[2].r = 0; green[2].g = 96;  green[2].b = 0; green[2].a = 255;

    blue[0].r = 0; blue[0].g = 0; blue[0].b = 255; blue[0].a = 255;
    blue[1].r = 0; blue[1].g = 0; blue[1].b = 128; blue[1].a = 255;
    blue[2].r = 0; blue[2].g = 0; blue[2].b = 96;  blue[2].a = 255;
  }

  void room::reset()
  {
    if(hasBoss && !bosses[hasBoss-1])
    {
      Boss.reset();
    }
  }

  void room::setBosses(purist* dood, short enable)
  {
    short x, i;
    short xpos, ypos;
    i = 0;
    for(x = 0; x < 4; ++x)
    {
      bosses[x] = dood->bosses[x];
      i+=bosses[x];
    }
    if(bosses[0])
      for(x = 0; x < 3; ++x)
      {
        if(enable)
        {
          for(xpos = 0; xpos < 320; ++xpos)
            for(ypos = 0; ypos < 240; ++ypos)
              if(!memcmp(&fore[ypos][xpos], &red[x], 4))
              {
                if(i < 3)
                  fore[ypos][xpos] = gray[x];
                else
                  fore[ypos][xpos] = white[x];
              }
          if(!memcmp(&color, &red[0], 3))
              color = gray[0];
        }
        red[x] = gray[x];
      }
    if(bosses[1])
      for(x = 0; x < 3; ++x) 
      {
        if(enable)
        {
          for(xpos = 0; xpos < 320; ++xpos)
            for(ypos = 0; ypos < 240; ++ypos)
              if(!memcmp(&fore[ypos][xpos], &green[x], 4))
              {
                if(i < 3)
                  fore[ypos][xpos] = gray[x];
                else
                  fore[ypos][xpos] = white[x];
              }
          if(!memcmp(&color, &green[0], 3))
            color = gray[0];
        }
        green[x] = gray[x];
      }
    if(bosses[2])
      for(x = 0; x < 3; ++x)
      {
        if(enable)
        {
          for(xpos = 0; xpos < 320; ++xpos)
            for(ypos = 0; ypos < 240; ++ypos)
              if(!memcmp(&fore[ypos][xpos], &blue[x], 4))
              {
                if(i < 3)
                  fore[ypos][xpos] = gray[x];
                else
                  fore[ypos][xpos] = white[x];
              }
          if(!memcmp(&color, &blue[0], 3))
            color = gray[0];
        }
        blue[x] = gray[x];
      }
    if(i == 3)
    {
      for(x = 0; x < 3; ++x)
      {
        red[x] = white[x];
        green[x] = white[x];
        blue[x] = white[x];
        color = white[0];
      }
    }
    if(enable)
    {
      gunz.setPointer(0);
      if(gunz.getLength() > 0) do
      {
        tempGun = ((gun*)gunz.getPointer());
        for(x = 0; x < 3; ++x)
          if(bosses[x])
            tempGun->neuterNum(x);
      } while(!gunz.incPointer());      
      glBindTexture(GL_TEXTURE_2D, image);
      glTexImage2D(GL_TEXTURE_2D, 0, 4, 320, 240, 0, GL_RGBA, GL_UNSIGNED_BYTE, fore);
    }

  }

  void room::refresh()
  {
    short x;
    short y;
    short i, j;
    float sum;

    memset(fore, 0, 320*240*4);

    for(x = 0; x < 320; ++x)
      for(y = 0; y < 240; ++y)
        fore[y][x].a = 255;

    for(y = 0; y < 60; ++y)
      for(x = 0; x < 80; ++x)
      {

        switch(getBlockBack2(x, y))
        {
          case 0x1:
            drawBlock(x, y, &(fore[0][0]), 2, red[2]);
          break;
          case 0x2:
            drawBlock(x, y, &(fore[0][0]), 2, green[2]);
          break;
          case 0x3:
            drawBlock(x, y, &(fore[0][0]), 2, blue[2]);
          break;
          case 0x4:
            drawBlock(x, y, &(fore[0][0]), 2, white[2]);
          break;
          default:
          break;
        }

        switch(getBlockBack1(x, y))
        {
          case 0x1:
            drawBlock(x, y, &(fore[0][0]), 1, red[1]);
          break;
          case 0x2:
            drawBlock(x, y, &(fore[0][0]), 1, green[1]);
          break;
          case 0x3:
            drawBlock(x, y, &(fore[0][0]), 1, blue[1]);
          break;
          case 0x4:
            drawBlock(x, y, &(fore[0][0]), 1, white[1]);
          break;
          default:
          break;
        }

        switch(getBlockFore(x, y))
        {
          case 0x1:
            drawBlock(x, y, &(fore[0][0]), 0, red[0]);
          break;
          case 0x2:
            drawBlock(x, y, &(fore[0][0]), 0, green[0]);
          break;
          case 0x3:
            drawBlock(x, y, &(fore[0][0]), 0, blue[0]);
          break;
          case 0x4:
            drawBlock(x, y, &(fore[0][0]), 0, white[0]);
          break;
          default:
          break;
        }

      }

    for(y = 0; y < 30; ++y)
      for(x = 0; x < 40; ++x)
      {
        memset(&miniData[y][x], 0, 3);
        miniData[y][x].a = 255;
        if(getBlockBack2(x*2, y*2))
          miniData[y][x] = white[2];
        if(getBlockBack1(x*2, y*2))
          miniData[y][x] = white[1];
        if(getBlockFore(x*2, y*2))
          miniData[y][x] = white[0];
      }


    glBindTexture(GL_TEXTURE_2D, image);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 320, 240, 0, GL_RGBA, GL_UNSIGNED_BYTE, fore);

    glBindTexture(GL_TEXTURE_2D, miniMap);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 40, 30, 0, GL_RGBA, GL_UNSIGNED_BYTE, miniData);
    

  }

  void room::load(short xpos, short ypos)
  {

    short x;
    short y;
    short i;
    short j;

    short gunNum = 0;
    short tempX, tempY;
    float tempR;
    unsigned char r, g, b;

    char* filename = new char[255];
    char* file = new char[255];
    char temp[256];

    strcpy(filename, "lvl/");
    if(xpos > 0)
    {
      sprintf(file, "%x", xpos);
      strcat(filename, file);
    }
    else
    {
      strcat(filename, "0");
    }
    if(ypos > 0)
    {
      sprintf(file, "%x", ypos);
      strcat(filename, file);
    }
    else
    {
      strcat(filename, "0");
    }
    strcat(filename, ".lvl");

    ifstream fin;
    fin.open(filename);
    if(!fin.is_open())
    {
      cout << "room: load operation failed for file " << filename << "." << endl;
      roomX = xpos/16;
      roomY = ypos;
      for(y = 0; y < 60; ++y)
        for(x = 0; x < 40; ++x)
        {
          backArray2[x][y] = 0;
          backArray1[x][y] = 0;
          foreArray[x][y] = 0;
        }
      refresh();
      return;
    }

    roomX = xpos;
    roomY = ypos;

    for(y = 0; y < 60; ++y)
      for(x = 0; x < 40; ++x)
        backArray2[x][y] = fin.get();
    for(y = 0; y < 60; ++y)
      for(x = 0; x < 40; ++x)
        backArray1[x][y] = fin.get();
    for(y = 0; y < 60; ++y)
      for(x = 0; x < 40; ++x)
        foreArray[x][y] = fin.get();

    if((xpos == 7 && ypos == 5 && !bosses[2]) || (xpos == 8 && ypos == 6 && !bosses[1]) || (xpos == 6 && ypos == 6 && !bosses[0]))
      for(y = 1; y < 25; ++y)
        for(x = 20; x < 60; ++x)
          addTile(4, 0, x, y);

    platNum = fin.get();

    if(platNum > 0)
    {
      color.r = fin.get();
      color.g = fin.get();
      color.b = fin.get();
      if(bosses[2] && bosses[1] && bosses[0])
        color = white[0];
      if((bosses[2] && color.b == 255 && color.r == 0) ||
         (bosses[1] && color.g == 255 && color.b == 0) ||
         (bosses[0] && color.r == 255 && color.b == 0))
        color = gray[0];

      fin.read((char*)(&size), 4);
      fin.read((char*)(&velocity), 4);
      fin.read((char*)(&maxDist), 4);
      initPlatforms();
    }

    saveNum = fin.get();
    if(saveNum > 0)
    {
      fin.read((char*)(&saveX), 2);
      fin.read((char*)(&saveY), 2);
    }

    gunNum = fin.get();
    while(gunNum > 0)
    {
        tempGun = new gun;
          fin.read((char*)(&tempX), 2);
          fin.read((char*)(&tempY), 2);
        tempGun->initialize(tempX, tempY);
          fin.read((char*)(&tempX), 2);
        tempGun->setBullets(tempX);
          r = fin.get(); g = fin.get(); b = fin.get();
        tempGun->setColor(r, g, b);
          r = fin.get(); g = fin.get(); b = fin.get();
        tempGun->setBulletColor(r, g, b);
        if((bosses[2] && b == 255) ||
           (bosses[1] && g == 255) ||
           (bosses[0] && r == 255))
          tempGun->neuter();
          r = fin.get();
          fin.read((char*)(&tempR), 4);
        tempGun->setFire(r, tempR);
          fin.read((char*)(&tempR), 4);
        tempGun->setRadius(tempR);
          r = fin.get();
        tempGun->setDamage(r);
        gunz.add(tempGun);
      --gunNum;
    }

    hasBoss = fin.get();
    if(hasBoss)
    {
      Boss.init();
      Boss.load(&fin);
      if(bosses[hasBoss-1])
      {
        Boss.kill();
      }
    }

    fin.close();

    refresh();
  }

  void room::save(short xpos, short ypos)
  {

    short x;
    short y;
    short i;
    short j;

    char* filename = new char[255];
    char* file = new char[255];

    strcpy(filename, "lvl/");
    if(xpos > 0)
    {
      sprintf(file, "%x", xpos);
      strcat(filename, file);
    }
    else
    {
      strcat(filename, "0");
    }
    if(ypos > 0)
    {
      sprintf(file, "%x", ypos);
      strcat(filename, file);
    }
    else
    {
      strcat(filename, "0");
    }
    strcat(filename, ".lvl");

    ofstream fout;
    fout.open(filename);
    if(!fout.is_open())
    {
      cout << "room: save operation failed for file" << filename << "." << endl;
      return;
    }

    for(y = 0; y < 60; ++y)
      for(x = 0; x < 40; ++x)
        fout.put(backArray2[x][y]);
    for(y = 0; y < 60; ++y)
      for(x = 0; x < 40; ++x)
        fout.put(backArray1[x][y]);
    for(y = 0; y < 60; ++y)
      for(x = 0; x < 40; ++x)
        fout.put(foreArray[x][y]);

    fout.put(platNum);

    if(platNum > 0)
    {
      fout.put(color.r);
      fout.put(color.g);
      fout.put(color.b);
      fout.write((char*)(&size), 4);
      fout.write((char*)(&velocity), 4);
      fout.write((char*)(&maxDist), 4);
    }

    fout.put(saveNum);
    if(saveNum > 0)
    {
      fout.write((char*)(&saveX), 2);
      fout.write((char*)(&saveY), 2);
    }

    fout.put(gunz.getLength());
    gunz.setPointer(0);
    if(gunz.getLength() > 0) do
    {
      tempGun = ((gun*)gunz.getPointer());
      tempGun->save(&fout);
    } while(!gunz.incPointer());

    fout.put(hasBoss);
    if(hasBoss)
      Boss.save(&fout);

    fout.close();

    refresh();
  }

  short room::playerSave(purist* dood, stuff* powerups)
  {
    if(saveNum == 0)
      return 0;

    if(abs(dood->xpos-2*saveX) <= 3 && abs(dood->ypos-2*saveY) <= 3)
    {
      dood->save(roomX, roomY);
      powerups->save();
      dood->visited[dood->roomX][dood->roomY] = 2;
      return 1;
    }
    return 0;
  }

  short room::warp(purist* dood)
  {
    if(saveNum == 0)
      return 0;
    dood->xpos = 2*saveX;
    dood->ypos = 2*saveY;
    dood->roomX = roomX;
    dood->roomY = roomY;
    return 1;
  }


  void room::drawBlock(short x, short y, Color* targ, char blocks, Color color)
  {
    short i;
    short j;

    for(i = 0; i < 4; ++i)
      for(j = 0; j < 4; ++j)
      {
        targ[320*(4*y+i)+4*x+j].r = 0;
        targ[320*(4*y+i)+4*x+j].g = 0;
        targ[320*(4*y+i)+4*x+j].b = 0;
        targ[320*(4*y+i)+4*x+j].a = 255;
      }

    if(y > 0)
    {
      if(x > 0)
      {
        if(getBlockTarg(x-1,y-1,blocks) == 0)
          targ[320*(4*y)+4*x] = color;
      }
      if(getBlockTarg(x,y-1,blocks) == 0)
        for(j = 0; j < 4; ++j)
          targ[320*(4*y)+4*x+j] = color;
      if(x < 79)
      {
        if(getBlockTarg(x+1,y-1,blocks) == 0)
          targ[320*(4*y)+4*x+3] = color;
      }   
    }

    if(x > 0)
    {
      if(getBlockTarg(x-1,y,blocks) == 0)
        for(i = 0; i < 4; ++i)
            targ[320*(4*y+i)+4*x] = color;
    }
    if(x < 79)
    {
      if(getBlockTarg(x+1,y,blocks) == 0)
        for(i = 0; i < 4; ++i)
          targ[320*(4*y+i)+4*x+3] = color;
    }   

    if(y < 59)
    {
      if(x > 0)
      {
        if(getBlockTarg(x-1,y+1,blocks) == 0)
          targ[320*(4*y+3)+4*x] = color;
      }
      if(getBlockTarg(x,y+1,blocks) == 0)
        for(j = 0; j < 4; ++j)
          targ[320*(4*y+3)+4*x+j] = color;
      if(x < 79)
      {
        if(getBlockTarg(x+1,y+1,blocks) == 0)
          targ[320*(4*y+3)+4*x+3] = color;
      }   
    }

  }

  void room::draw()
  {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image);

    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
      glTexCoord2d(0, 0); glVertex2d(0, 720);
      glTexCoord2d(1, 0); glVertex2d(960, 720);
      glTexCoord2d(1, 1); glVertex2d(960, 0);
      glTexCoord2d(0, 1); glVertex2d(0, 0);
    glEnd();

    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);

    drawPlatforms();

    if(!hasBoss || (hasBoss && !(Boss.getKilled() == 2)))
    {
      gunz.setPointer(0);
      if(gunz.getLength() > 0) do
      {
        tempGun = ((gun*)gunz.getPointer());
        if(hasBoss && (Boss.getKilled()==1 || Boss.getHurt()))
          tempGun->clearBullets();
        tempGun->drawGun();
        tempGun->drawBullets();
      } while(!gunz.incPointer());
    }

    if(saveNum > 0)
    {
      switch((short)floor(savePhase/5.0))
      {
        case 0:
          glColor4f(.375,.375,.375,1);
        break;
        case 1:
          glColor4f(.25,.25,.25,1);
        break;
        case 2:
          glColor4f(.75,.75,.75,1);
        break;
      }
      glBegin(GL_QUADS);
        glVertex2d(6*saveX-3, 720-6*saveY-3);
        glVertex2d(6*saveX-3, 720-6*saveY+3);
        glVertex2d(6*saveX+3, 720-6*saveY+3);
        glVertex2d(6*saveX+3, 720-6*saveY-3);
      glEnd();

      switch((short)floor(savePhase/5.0))
      {
        case 0:
          glColor4f(.75,.75,.75,1);
        break;
        case 1:
          glColor4f(.375,.375,.375,1);
        break;
        case 2:
          glColor4f(.25,.25,.25,1);
        break;
      }
      glBegin(GL_QUADS);
        glVertex2d(6*saveX-6, 720-6*saveY-6);
        glVertex2d(6*saveX-6, 720-6*saveY+6);
        glVertex2d(6*saveX+6, 720-6*saveY+6);
        glVertex2d(6*saveX+6, 720-6*saveY-6);
      glEnd();

    }

    if(roomX == 7 && roomY == 6 && !(bosses[0] && bosses[1] && bosses[2]))
    {
      glBegin(GL_QUADS);
        glColor3f(1,1,1);
        glVertex2d(456, 19);
        glVertex2d(468, 19);
        glVertex2d(468, 17);
        glVertex2d(456, 17);
        glVertex2d(492, 19);
        glVertex2d(504, 19);
        glVertex2d(504, 17);
        glVertex2d(492, 17);
        glColor3f(.5,.5,.5);
        glVertex2d(456, 21);
        glVertex2d(468, 21);
        glVertex2d(468, 15);
        glVertex2d(456, 15);
        glVertex2d(492, 21);
        glVertex2d(504, 21);
        glVertex2d(504, 15);
        glVertex2d(492, 15);
        glColor3f(.25,.25,.25);
        glVertex2d(456, 23);
        glVertex2d(468, 23);
        glVertex2d(468, 13);
        glVertex2d(456, 13);
        glVertex2d(492, 23);
        glVertex2d(504, 23);
        glVertex2d(504, 13);
        glVertex2d(492, 13);
      glEnd();
    }

    glClear(GL_DEPTH_BUFFER_BIT);

    if(hasBoss)
      Boss.draw();

  }
  void room::drawMap(short x, short y)
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, miniMap);

    glColor4f(1,1,1,1);
    if(saveNum > 0)
      glColor4f(1,.5,.5,1);
    glBegin(GL_QUADS);
      glTexCoord2d(0, 0); glVertex2d(x, y+30);
      glTexCoord2d(1, 0); glVertex2d(x+40, y+30);
      glTexCoord2d(1, 1); glVertex2d(x+40, y);
      glTexCoord2d(0, 1); glVertex2d(x, y);
    glEnd();

    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);
  }


  void room::update()
  {
    updatePlatforms();
    if(saveNum > 0)
    {
      ++savePhase;
      savePhase%=14;
    }

    if(!hasBoss || (hasBoss && !(Boss.getKilled()==2)))
    {
      gunz.setPointer(0);
      if(gunz.getLength() > 0) do
      {
        tempGun = ((gun*)gunz.getPointer());
        tempGun->update();
      } while(!gunz.incPointer());
    }
  }

  short room::collide(purist* dood)
  {

    double xpos, ypos;
    double xvel, yvel; 
    double vel, endX, endY;
    short hit = 0;
    short wasGround;

    short left = 0;

    short x, y, i;

    bullet* bullets;
    short bulletMax;

    xpos = dood->xpos;
    ypos = dood->ypos;

    xvel = dood->xvel;
    yvel = dood->yvel;


    xpos = dood->xpos;
    ypos = dood->ypos;

    xvel = dood->xvel;
    yvel = dood->yvel;

    if(getBlock((short)(xpos+xvel), (short)(ypos)) || (hasBoss?Boss.checkBlock((short)(xpos+xvel), (short)(ypos)):0))
    {
      if(xvel > 0)
      {
        xpos = 4*floor((xpos+xvel)/4)-.2;
        xvel = 0;
      }
      if(xvel < 0)
      {
        xpos = 4*floor(1+(xpos+xvel)/4)+.1;
        xvel = 0;
      }
    }

    xpos+=xvel;

    dood->xvel = xvel;
    dood->xpos = xpos;

    wasGround = dood->onGround;
    dood->onGround = 0;
    if(platNum > 0)
    {
      for(i = 0; i < 15; ++i)
      {
        if(xpos <= platX[i]+size && xpos >= platX[i]-size)
        {
          if(ypos <= platY[i] && ypos+yvel >= platY[i])
          {
            ypos = platY[i]-1.5;
            yvel = velocity+1;
            dood->onGround = 1;
            dood->jumps = 0;
          }
        }
      }
    }
    if(getBlock((short)(xpos), (short)(ypos+yvel)) || (hasBoss?Boss.checkBlock((short)(xpos), (short)(ypos+yvel)):0))
    {
      if(yvel > 0)
      {
        ypos = 4*floor((ypos+yvel)/4)-1.5;
        yvel = 1;
        dood->onGround = 1;
        dood->jumps = 0;
      }
      if(yvel < 0)
      {
        ypos = 4*floor(1+(ypos+yvel)/4)+.5;
        yvel = 0;
      }
    }

    if(roomX == 7 && roomY == 6 && !(bosses[0] && bosses[1] && bosses[2]) && yvel > 0 && xpos > 100 && xpos < 220)
      if(ypos + yvel > 232)
      {
        ypos = 232;
        yvel = 1;
        dood->onGround = 1;
        dood->jumps = 0;
      }


    if(edgeCounter > 0)
      --edgeCounter;
    if(wasGround && !(dood->onGround) && yvel > 0)
    {
      yvel = 0;
      edgeCounter = 3;
    }
    if(!wasGround && !(dood->onGround) && dood->jumps == 0 && edgeCounter == 0)
      dood->jumps = 1;

    ypos+=yvel;

    dood->yvel = yvel;
    dood->ypos = ypos;

    if(hasBoss)
      Boss.collide(dood);

    //Room Transitions

    if(hasBoss && !(Boss.getKilled() == 2))
    {
      if(xpos+xvel < 1)
      {
        xvel = 0;
        xpos = 1;
      }
      if(xpos+xvel >318)
      {
        xvel = 0;
        xpos = 318;
      }
      if(ypos+yvel < 1)
      {
        yvel = 0;
        ypos = 1;
      }
      if(ypos+yvel >238)
      {
        yvel = 0;
        ypos = 238;
      }
    }

    if(xpos + xvel < 0)
    {
      if(roomX > 0)
      {
        dood->xpos = 319;
        --dood->roomX;
        if(dood->visited[dood->roomX][dood->roomY] == 0)
          dood->visited[dood->roomX][dood->roomY] = 1;
        left = 1;
      }
      else
      {
        dood->xvel =0;
        dood->xpos = 0;
      }
    }
    if(xpos + xvel > 319)
    {
      if(roomX < 14)
      {
        dood->xpos = 0;
        ++dood->roomX;
        if(dood->visited[dood->roomX][dood->roomY] == 0)
          dood->visited[dood->roomX][dood->roomY] = 1;
        left = 1;
      }
      else
      {
        dood->xvel =0;
        dood->xpos = 319;
      }
    }

    if(ypos + yvel < 0 )
    {
      if(roomY > 0)
      {
        dood->ypos = 239;
        --dood->roomY;
        if(dood->visited[dood->roomX][dood->roomY] == 0)  
          dood->visited[dood->roomX][dood->roomY] = 1;
        left = 1;
      }
      else
      {
        dood->yvel =0;
        dood->ypos = 0;
      }
    }
    if(ypos + yvel > 239 )
    {
      dood->ypos = 0;
      ++dood->roomY;
      if(dood->visited[dood->roomX][dood->roomY] == 0)
        dood->visited[dood->roomX][dood->roomY] = 1;
      left = 1;
    }

    //End Room Transitions

//gun collisions
    if(hasBoss && Boss.getKilled())
    {
      dood->bosses[hasBoss-1] = 1;
    }
    if(!hasBoss || (hasBoss && !(Boss.getKilled())))
    {
      gunz.setPointer(0);
      if(gunz.getLength() > 0) do
      {
        tempGun = ((gun*)gunz.getPointer());
        tempGun->updateBullets(dood->xpos,dood->ypos);
        bulletMax = tempGun->getNum();
        bullets = tempGun->getBullets();
        for(x = 0; x < bulletMax; ++x)
          if(getBlockFore((short)(bullets[x].x/12),(short)(bullets[x].y/12)))
            bullets[x].active = 0;
        dood->health-=tempGun->checkCollision(dood->xpos,dood->ypos);
        if(left)
          tempGun->clearBullets();
      } while(!gunz.incPointer());
    }

    return hit;

  }

  char room::getBlock(short x, short y)
  {
    return getBlockFore(x/4, y/4);
  }

  char room::getBlockTarg(short x, short y, char targ)
  {
    switch(targ)
    {
    case 0:
      return getBlockFore(x, y);
    break;
    case 1:
      return getBlockBack1(x, y);
    break;
    case 2:
      return getBlockBack2(x, y);
    break;
    }
  }

  char room::getBlockFore(short x, short y)
  {
    if(x%2 == 0)
    {
      return ((foreArray[(short)(x/2)][y] & 0xF0) >> 4);
    }
    else
    {
      return (foreArray[(short)(x/2)][y] & 0x0F); 
    }
  }

  char room::getBlockBack1(short x, short y)
  {
    if(x%2 == 0)
    {
      return ((backArray1[(short)(x/2)][y] & 0xF0) >> 4);
    }
    else
    {
      return (backArray1[(short)(x/2)][y] & 0x0F); 
    }
  }

  char room::getBlockBack2(short x, short y)
  {
    if(x%2 == 0)
    {
      return ((backArray2[(short)(x/2)][y] & 0xF0) >> 4);
    }
    else
    {
      return (backArray2[(short)(x/2)][y] & 0x0F); 
    }
  }

  short room::getX()
  {
    return roomX;
  }

  short room::getY()
  {
    return roomY;
  }

  void room::set(short x, short y, short layer, char color)
  {
    if(x%2 == 0)
    {
      switch(layer)
      {
      case 0:
        backArray2[(short)(x/2)][y] &= 0x0F;
        backArray2[(short)(x/2)][y] |= (color << 4);
      break;
      case 1:
        backArray1[(short)(x/2)][y] &= 0x0F;
        backArray1[(short)(x/2)][y] |= (color << 4);
      break;
      case 2:
        foreArray[(short)(x/2)][y] &= 0x0F;
        foreArray[(short)(x/2)][y] |= (color << 4);
      break;
      }
    }
    else
    {
      switch(layer)
      {
      case 0:
        backArray2[(short)(x/2)][y] &= 0xF0;
        backArray2[(short)(x/2)][y] |= (color);
      break;
      case 1:
        backArray1[(short)(x/2)][y] &= 0xF0;
        backArray1[(short)(x/2)][y] |= (color);
      break;
      case 2:
        foreArray[(short)(x/2)][y] &= 0xF0;
        foreArray[(short)(x/2)][y] |= (color);
      break;
      }
    }

  }

  short room::hasABoss()
  {
    return (hasBoss && Boss.getKilled()!=2);
  }

  void room::initPlatforms()
  {
    short x;
    double temp;
    platCenter = rand()%320;
    platDir = 0;
    platX[0] = (319-size*2)*(rand()/(double)RAND_MAX) + size;
    platY[0] = 16*15;
    for(x = 1; x < 15; ++x)
    {
        temp = 2*(rand()/(double)RAND_MAX)-1;
        if(platDir)
          platCenter+=maxDist/4;
        else
          platCenter-=maxDist/4;
        if(platCenter < maxDist/2)
          platDir = 1;
        if(platCenter > 320-maxDist/2)
          platDir = 0;

        platX[x] = platCenter+(maxDist-maxDist/3)*temp + (maxDist/3)*abs(temp)/temp;
        if(platX[x] < size)
          platX[x] = size + maxDist*rand()/(double)RAND_MAX;
        if(platX[x] > 319-size)
          platX[x] = 319-size-maxDist*rand()/(double)RAND_MAX;
      platY[x] = 16*(double)(15-x);
    }
  }

  void room::updatePlatforms()
  {
    if(platNum == 0)
    {
      return;
    }

    short x;
    short i;
    short j;
    double temp;

    for(x = 0; x < 15; ++x)
    {
      platY[x] += velocity;
      if(platY[x] > 250)
      {
        platY[x] = 0;
        temp = 2*(rand()/(double)RAND_MAX)-1;
        if(platDir)
          platCenter+=maxDist/4;
        else
          platCenter-=maxDist/4;
        if(platCenter < maxDist/2)
          platDir = 1;
        if(platCenter > 320-maxDist/2)
          platDir = 0;
        platX[x] = platCenter+(maxDist-maxDist/3)*temp + (maxDist/3)*abs(temp)/temp;

        if(platX[x] < size)
          platX[x] = size + maxDist*rand()/(double)RAND_MAX;
        if(platX[x] > 319-size)
          platX[x] = 319-size - maxDist*rand()/(double)RAND_MAX;
      }
    }
  }

  void room::drawPlatforms()
  {

    if(platNum == 0)
      return;

    short i;
    glBegin(GL_QUADS);
    for(i = 0; i < 15; ++i)
    {
      glColor4ub(color.r,color.g,color.b,255);
      glVertex2d(3*platX[i]-3*size, 720-3*platY[i]-1);
      glVertex2d(3*platX[i]-3*size, 720-3*platY[i]+1);
      glVertex2d(3*platX[i]+3*size, 720-3*platY[i]+1);
      glVertex2d(3*platX[i]+3*size, 720-3*platY[i]-1);
      glColor4ub(color.r,color.g,color.b,128);
      glVertex2d(3*platX[i]-3*size, 720-3*platY[i]+1);
      glVertex2d(3*platX[i]-3*size, 720-3*platY[i]+3);
      glVertex2d(3*platX[i]+3*size, 720-3*platY[i]+3);
      glVertex2d(3*platX[i]+3*size, 720-3*platY[i]+1);
      glColor4ub(color.r,color.g,color.b,82);
      glVertex2d(3*platX[i]-3*size, 720-3*platY[i]+3);
      glVertex2d(3*platX[i]-3*size, 720-3*platY[i]+5);
      glVertex2d(3*platX[i]+3*size, 720-3*platY[i]+5);
      glVertex2d(3*platX[i]+3*size, 720-3*platY[i]+3);
    }
    glEnd();

    glClear(GL_DEPTH_BUFFER_BIT);
  }

  void room::clearPlatforms()
  {
    platNum = 0;
  }

  void room::addPlatforms(short newColor)
  {
    switch(newColor)
    {
    case 0:
      platNum = 0;
    break;
    case 1:
      platNum = 1;
      initPlatforms();
      color.r = 255;
      color.g = 0;
      color.b = 0;
    break;
    case 2:
      platNum = 1;
      initPlatforms();
      color.r = 0;
      color.g = 255;
      color.b = 0;
    break;
    case 3:
      platNum = 1;
      initPlatforms();
      color.r = 0;
      color.g = 0;
      color.b = 255;
    break;
    case 4:
      platNum = 1;
      initPlatforms();
      color.r = 255;
      color.g = 255;
      color.b = 255;
    break;
    }
  }

  void room::adjustPlatforms(short xpos, short ypos)
  {
    switch(xpos/16)
    {
    case 0:
      if(ypos/16 == 0 && size > .5)
        size -=.5;
      else if(maxDist >5)
      {
        maxDist-=5;
        initPlatforms();
      }
    break;
    case 1:
      if(ypos/16 == 0)
        size +=.5;
      else
      {
        maxDist+=5;
        initPlatforms();
      }
    break;
    case 2:
      if(ypos/16 == 0)
      {
        if(velocity < 2)
          velocity +=.05;
      }
      else
        velocity -=.05;
    break;
    }
  }

  void room::addSave(short xpos, short ypos)
  {
    saveNum = 1;
    saveX = xpos/6;
    saveY = ypos/6;
  }
  void room::erase(short xpos, short ypos)
  {
    if(xpos/6 == saveX && ypos/6 == saveY)
      saveNum = 0;

    gunz.setPointer(0);
    if(gunz.getLength() > 0) do
    {
      tempGun = ((gun*)gunz.getPointer());
      if(tempGun->checkPos(xpos, ypos))
      {
        gunz.delPointer();
        delete tempGun;
      }

    } while(!gunz.incPointer());

  }

  void room::addBossTile(short type, short xpos, short ypos)
  {
    if(!hasBoss)
    {
      hasBoss = 1;
      Boss.init();
    }
    Boss.drawBlock(xpos - 20, ypos - 1, type);
  }
  void room::addBossOde(short xpos, short ypos)
  {
    if(!hasBoss)
    {
      hasBoss = 1;
      Boss.init();
    }
    Boss.addOde(xpos, ypos);
  }

  void room::addTile(short type, short layer, short xpos, short ypos)
  {
    switch(layer)
    {
    case 0:
      if(xpos%2==0)
      {
        foreArray[xpos/2][ypos] &= 0x0F;
        foreArray[xpos/2][ypos] |= ((type << 4)&0xF0);
      }
      else
      {
        foreArray[xpos/2][ypos] &= 0xF0;
        foreArray[xpos/2][ypos] |= (type&0x0F);
      }
    break;
    case 1:
      if(xpos%2==0)
      {
        backArray1[xpos/2][ypos] &= 0x0F;
        backArray1[xpos/2][ypos] |= ((type << 4)&0xF0);
      }
      else
      {
        backArray1[xpos/2][ypos] &= 0xF0;
        backArray1[xpos/2][ypos] |= (type&0x0F);
      }
    break;
    case 2:
      if(xpos%2==0)
      {
        backArray2[xpos/2][ypos] &= 0x0F;
        backArray2[xpos/2][ypos] |= ((type << 4)&0xF0);
      }
      else
      {
        backArray2[xpos/2][ypos] &= 0xF0;
        backArray2[xpos/2][ypos] |= (type&0x0F);
      }
    break;
    }

  }

  void room::addGun(short xpos, short ypos, gun *nGun)
  {
    nGun->clearBullets();
    nGun->setPos(xpos, ypos);
    gunz.add(nGun);
  }




