#include <cstdlib> 
#include <iostream>
#include <cmath>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glpng.h>
#include "sfont.h"
#include "purist.h"
#include "room.h"
#include "gun.h"

using namespace std;

#define FRAMERATE 16

void glInit(short w, short h);
short checkPoint(short xpos, short ypos, short left, short top, short w, short h);

int main(int argc,char *argv[])
{

  long start, stop;
  GLuint splash1, splash2;
  GLuint editLogo;
  GLuint colors, tools, no, plat;

  short tool = 0;
  short color = 0;
  short shade = 0;
  short platColor = 4;

  short gunColor = 0;

  short x, y, i, j;
  int mousex, mousey;
  short click = 0;
  short tClick = 0;
  short stopClick = 0;

  short e = 0;

  short xpos = 7;
  short ypos = 6;

  short selX, selY, selW, selH;
  short sel = 0;
  short grid = 0;

  char posText[256];

  /******************/
  /* Graphical Init */
  /******************/

  glInit(1072,720);

  SDL_Surface* temp;


  temp = SDL_LoadBMP("bmp/editLogo.bmp");

  glGenTextures(1, &editLogo);
  glBindTexture(GL_TEXTURE_2D, editLogo);

  glTexImage2D(GL_TEXTURE_2D, 0, 4, temp->w, temp->h, 0, 32992, GL_UNSIGNED_BYTE, temp->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  temp = SDL_LoadBMP("bmp/colors.bmp");
  glGenTextures(1, &colors);
  glBindTexture(GL_TEXTURE_2D, colors);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, temp->w, temp->h, 0, 32992, GL_UNSIGNED_BYTE, temp->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  temp = SDL_LoadBMP("bmp/tools.bmp");
  glGenTextures(1, &tools);
  glBindTexture(GL_TEXTURE_2D, tools);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, temp->w, temp->h, 0, 32992, GL_UNSIGNED_BYTE, temp->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  temp = SDL_LoadBMP("bmp/plat.bmp");
  glGenTextures(1, &plat);
  glBindTexture(GL_TEXTURE_2D, plat);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, temp->w, temp->h, 0, 32992, GL_UNSIGNED_BYTE, temp->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  sfont font;
  font.init();
  font.load("data/SLGFont2.srf");

  SDL_Event event;
  Uint8* keys;
  Uint8 mouseState;

  room** rooms;
  rooms = new room*[15];
  for(x = 0; x < 15; ++x)
    rooms[x] = new room[13];

  for(x = 0; x < 15; ++x)
    for(y = 0; y < 13; ++y)
    {
      rooms[x][y].initialize();
      rooms[x][y].load(x, y);
    }

  gun* exampleGun;
  exampleGun = new gun;
  exampleGun->initialize(84, 16);
  exampleGun->setColor(0,0,0);
  exampleGun->setBulletColor(128,128,128);

  gun* addGun;

  stuff powerups;
  powerups.initialize();
  powerups.clear();

  while(e==0)
  {

    start = clock();
    keys=SDL_GetKeyState(NULL);
    mouseState = SDL_GetMouseState(&mousex, &mousey);    

    if(keys[SDLK_ESCAPE])
      e=1;

    if(keys[SDLK_LEFT])
    {
      keys[SDLK_LEFT] = 0;
      if(xpos > 0)
        --xpos;
    }
    if(keys[SDLK_RIGHT])
    {
      keys[SDLK_RIGHT] = 0;
      if(xpos < 14)
        ++xpos;
    }
    if(keys[SDLK_UP])
    {
      keys[SDLK_UP] = 0;
      if(ypos > 0)
        --ypos;
    }
    if(keys[SDLK_DOWN])
    {
      keys[SDLK_DOWN] = 0;
      if(ypos < 12)
        ++ypos;
    }

    if(keys[SDLK_s] && (keys[SDLK_LCTRL] || keys[SDLK_RCTRL]))
    {
      keys[SDLK_s] = 0;
      rooms[xpos][ypos].save(xpos, ypos);
      powerups.save();
    }
    if(keys[SDLK_g] && (keys[SDLK_LCTRL] || keys[SDLK_RCTRL]))
    {
      keys[SDLK_g] = 0;
      ++grid;
      grid%=2;
    }

    while(SDL_PollEvent(&event))
    {
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
      tClick = 1;
    }

    click = 0;
    if(mouseState && SDL_BUTTON(1))
    {
      if(mousex < 960 && tool == 1 && sel == 0)
      {
        selX = mousex/12;
        selY = mousey/12;
        sel = 1;
      }
      tClick = 1;
      click = 1;
    }

    if(event.type == SDL_MOUSEBUTTONUP)
    {cout << "release" << endl;
      tClick = 0;
      stopClick = 0;
    }
    }

    if(mousex > 960 && tClick)
    {
      tClick = 0;
      if(checkPoint(mousex, mousey, 968, 96, 64, 128))  //Pick tool
      {
        tool = ((mousex-968)/32)+2*((mousey-96)/32);
      }
      else if(checkPoint(mousex, mousey, 968, 256, 60, 36))  //Pick Color
      {
        color = (mousex-968)/12;
        shade = (mousey-256)/12;
      }
      else if(checkPoint(mousex, mousey, 968, 324, 60, 36))  //Pick Platform
      {
        platColor = (mousex-968)/12;
        rooms[xpos][ypos].addPlatforms(platColor);
      }
      else if(checkPoint(mousex, mousey, 968, 376, 48, 32))  //Adjust Platform
      {
        rooms[xpos][ypos].adjustPlatforms(mousex-968, mousey-376);
      }
      else if(checkPoint(mousex, mousey, 968, 440, 60, 36))  //Pick Gun
      {
        gunColor = (mousex-968)/12;
        switch(gunColor)
        {
        case 0:
          exampleGun->setColor(0,0,0);
          exampleGun->setBulletColor(128,128,128);
        break;
        case 1:
          exampleGun->setColor(255,0,0);
          exampleGun->setBulletColor(255,0,0);
        break;
        case 2:
          exampleGun->setColor(0,128,0);
          exampleGun->setBulletColor(0,255,0);
        break;
        case 3:
          exampleGun->setColor(0,128,255);
          exampleGun->setBulletColor(0,128,255);
        break;
        case 4:
          exampleGun->setColor(192,192,192);
          exampleGun->setBulletColor(255,255,255);
        break;
        }
      }
      else if(checkPoint(mousex, mousey, 968, 492, 48, 32))  //Adjust Gun
      {
        exampleGun->adjust(mousex-968, mousey-492);
      }
    }

    if(!click && sel)
    {
      if(mousex < 960)
      {
        if(mousex/12 < selX)
        {
          selW = selX - mousex/12;
          selX = mousex/12;
        }
        else
          selW = mousex/12 - selX +1;
        if(mousey/12 < selY)
        {
          selH = selY - mousey/12;
          selY = mousey/12;
        }
        else
          selH = mousey/12 - selY + 1;
        for(x = selX; x < selX+selW; ++x)
          for(y = selY; y < selY+selH; ++y)
            rooms[xpos][ypos].addTile(color, shade, x, y);
        rooms[xpos][ypos].refresh();  
      }
      sel = 0;    
    }

    if(click && mousex < 960)
    {
      switch(tool)
      {
      case 0:
        rooms[xpos][ypos].addTile(color, shade, mousex/12, mousey/12);
        rooms[xpos][ypos].refresh();
      break;
      case 2:
        rooms[xpos][ypos].addSave(mousex, mousey);
      break;
      case 6:
        rooms[xpos][ypos].addBossTile(color, mousex/12, mousey/12);
      break;
      }
    }
    if(tClick && !stopClick && mousex < 960)
    {cout << "tap" << endl;
      stopClick = 1;
      tClick = 0;
      switch(tool)
      {
      case 3:
        powerups.addPowerup(color, mousex, mousey, xpos, ypos);
      break;
      case 4:
        addGun = new gun;
        exampleGun->copy(addGun);
        rooms[xpos][ypos].addGun(mousex, mousey, addGun);
      break;
      case 5:
        rooms[xpos][ypos].erase(mousex, mousey);
        powerups.erase(xpos, ypos, mousex, mousey);
      break;
      case 7:
        rooms[xpos][ypos].addBossOde(mousex, mousey);
      break;
      }
    }

    for(x = 0; x < 15; ++x)
      for(y = 0; y < 13; ++y)
        rooms[x][y].update();

    exampleGun->update();

    /******************/
    /* Graphics Phase */
    /******************/

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);
    glLoadIdentity();			         	// Reset The View
    glEnable(GL_BLEND);

    glColor3f(.5,.5,.5);
    glBegin(GL_LINES);
      glVertex2d(960,0);
      glVertex2d(960,720);
    glEnd();

    powerups.drawPowerups(xpos, ypos);

    rooms[xpos][ypos].draw();

    if(tool == 6)
    {
      glColor4f(1,1,1,.5);
      glBegin(GL_LINE_LOOP);
        glVertex2d(240,720-12);
        glVertex2d(240,719-300);
        glVertex2d(720,720-300);
        glVertex2d(720,720-12);
      glEnd();
    }

    if(mousex < 960)
    {
      if(tool == 3)
      {
        glColor4f(1,1,1,.5);
        glBegin(GL_QUADS);
          glVertex2d(mousex-6, 720-mousey-6);
          glVertex2d(mousex-6, 720-mousey+6);
          glVertex2d(mousex+6, 720-mousey+6);
          glVertex2d(mousex+6, 720-mousey-6);          
        glEnd();
      }
      if(tool == 4)
      {
        glColor4f(1,1,1,.5);
        glBegin(GL_QUADS);
          glVertex2d(mousex-4, 720-mousey-4);
          glVertex2d(mousex-4, 720-mousey+4);
          glVertex2d(mousex+4, 720-mousey+4);
          glVertex2d(mousex+4, 720-mousey-4);          
        glEnd();
      }
      if(tool == 7)
      {
        glColor4f(1,1,1,.5);
        glBegin(GL_QUADS);
          glVertex2d(mousex-4, 720-mousey-4);
          glVertex2d(mousex-4, 720-mousey+4);
          glVertex2d(mousex+4, 720-mousey+4);
          glVertex2d(mousex+4, 720-mousey-4);          
        glEnd();
      }    }
    if(sel)
    {
        glColor4f(1,1,1,.5);
        glBegin(GL_LINE_LOOP);
          glVertex2d(12*selX, 720-12*selY);
          glVertex2d(mousex, 720-12*selY);
          glVertex2d(mousex, 720-mousey);
          glVertex2d(12*selX, 720-mousey);
        glEnd();
    }

    if(keys[SDLK_m])
    {
      for(x = 0; x < 15; ++x)
        for(y = 0; y < 13; ++y)
          rooms[x][y].drawMap(180+x*40, 165+(12-y)*30);

      glColor4f(.5,.5,.5,1);
      glBegin(GL_LINE_LOOP);
        glVertex2d(179, 165);
        glVertex2d(180, 555);
        glVertex2d(781, 555);
        glVertex2d(781, 165);
      glEnd();
      glColor4f(1,1,0,1);
      glBegin(GL_LINE_LOOP);
        glVertex2d(179+xpos*40, 165+(12-ypos)*30);
        glVertex2d(180+xpos*40, 165+(12-ypos)*30+30);
        glVertex2d(180+xpos*40+40, 165+(12-ypos)*30+30);
        glVertex2d(180+xpos*40+40, 165+(12-ypos)*30);
      glEnd();
    }

    if(grid)
    {
      glColor4f(1,1,1,.25);
      glBegin(GL_LINES);
        for(x = 0; x < 80; ++x)
        {
          glVertex2d(12*x, 0);
          glVertex2d(12*x, 720);
        }
        for(y = 0; y < 80; ++y)
        {
          glVertex2d(0, 12*y);
          glVertex2d(960, 12*y);
        }
      glEnd();
    }

    glTranslated(960, 0, 0);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, editLogo);
    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
      glTexCoord2d(0, 0); glVertex2d(16, 704);
      glTexCoord2d(.625, 0); glVertex2d(96, 704);
      glTexCoord2d(.625, .75); glVertex2d(96, 656);
      glTexCoord2d(0, .75); glVertex2d(16, 656);
    glEnd();

    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);

    glTranslated(0, -96, 0);
    {
      glColor3f(1,1,1);
      font.text(8, 740, "Tool:");
      glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tools);
        glBegin(GL_QUADS);
          glTexCoord2d(0, 0); glVertex2d(8, 720);
          glTexCoord2d(1, 0); glVertex2d(72, 720);
          glTexCoord2d(1, 1); glVertex2d(72, 592);
          glTexCoord2d(0, 1); glVertex2d(8, 592);
        glEnd();
        glClear(GL_DEPTH_BUFFER_BIT);
      glDisable(GL_TEXTURE_2D);
      glColor3f(1,1,0);
      glBegin(GL_LINE_LOOP);
        glVertex2d(9+32*(tool%2), 720-32*(tool/2));
        glVertex2d(40+32*(tool%2), 720-32*(tool/2));
        glVertex2d(40+32*(tool%2), 689-32*(tool/2));
        glVertex2d(8+32*(tool%2), 689-32*(tool/2));
      glEnd();
    }

    glTranslated(0, -160, 0);
    {
      glColor3f(1,1,1);
      font.text(8, 740, "Color:");
      glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, colors);
        glBegin(GL_QUADS);
          glTexCoord2d(0, 0); glVertex2d(8, 720);
          glTexCoord2d(.9375, 0); glVertex2d(68, 720);
          glTexCoord2d(.9375, .5625); glVertex2d(68, 684);
          glTexCoord2d(0, .5625); glVertex2d(8, 684);
        glEnd();
        glClear(GL_DEPTH_BUFFER_BIT);
      glDisable(GL_TEXTURE_2D);
      glColor3f(1,1,0);
      glBegin(GL_LINE_LOOP);
        glVertex2d(9+12*color, 720-12*shade);
        glVertex2d(20+12*color, 720-12*shade);
        glVertex2d(20+12*color, 709-12*shade);
        glVertex2d(8+12*color, 709-12*shade);
      glEnd();
    }

    glTranslated(0, -68, 0);
    {
      glColor3f(1,1,1);
      font.text(8, 740, "Platforms:");
      glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, colors);
        glBegin(GL_QUADS);
          glTexCoord2d(0, 0); glVertex2d(8, 720);
          glTexCoord2d(.9375, 0); glVertex2d(68, 720);
          glTexCoord2d(.9375, .5625); glVertex2d(68, 684);
          glTexCoord2d(0, .5625); glVertex2d(8, 684);
        glEnd();
        glClear(GL_DEPTH_BUFFER_BIT);
      glDisable(GL_TEXTURE_2D);
      glColor3f(1,1,0);
      glBegin(GL_LINE_LOOP);
        glVertex2d(9+12*platColor, 720);
        glVertex2d(20+12*platColor, 720);
        glVertex2d(20+12*platColor, 685);
        glVertex2d(8+12*platColor, 685);
      glEnd();
    }

    glTranslated(0, -52, 0);
    {
      glColor3f(1,1,1);
      glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, plat);
        glBegin(GL_QUADS);
          glTexCoord2d(0, 0); glVertex2d(8, 720);
          glTexCoord2d(.75, 0); glVertex2d(56, 720);
          glTexCoord2d(.75, 1); glVertex2d(56, 688);
          glTexCoord2d(0, 1); glVertex2d(8, 688);
        glEnd();
        glClear(GL_DEPTH_BUFFER_BIT);
      glDisable(GL_TEXTURE_2D);
    }

    glTranslated(0, -64, 0);
    {
      glColor3f(1,1,1);
      font.text(8, 740, "Gun Stat:");
      glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, colors);
        glBegin(GL_QUADS);
          glTexCoord2d(0, 0); glVertex2d(8, 720);
          glTexCoord2d(.9375, 0); glVertex2d(68, 720);
          glTexCoord2d(.9375, .5625); glVertex2d(68, 684);
          glTexCoord2d(0, .5625); glVertex2d(8, 684);
        glEnd();
        glClear(GL_DEPTH_BUFFER_BIT);
      glDisable(GL_TEXTURE_2D);
      glColor3f(1,1,0);
      glBegin(GL_LINE_LOOP);
        glVertex2d(9+12*gunColor, 720);
        glVertex2d(20+12*gunColor, 720);
        glVertex2d(20+12*gunColor, 685);
        glVertex2d(8+12*gunColor, 685);
      glEnd();
    }

    glTranslated(0, -52, 0);
    {
      glColor3f(1,1,1);
      glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, plat);
        glBegin(GL_QUADS);
          glTexCoord2d(0, 0); glVertex2d(8, 720);
          glTexCoord2d(.75, 0); glVertex2d(56, 720);
          glTexCoord2d(.75, 1); glVertex2d(56, 688);
          glTexCoord2d(0, 1); glVertex2d(8, 688);
        glEnd();
        glClear(GL_DEPTH_BUFFER_BIT);
      glDisable(GL_TEXTURE_2D);
      exampleGun->drawGun();
        exampleGun->updateBullets(mousex-1016, mousey-460);
        exampleGun->limit();
      exampleGun->drawBullets();
    }
    glTranslated(0, -80, 0);

    glColor3f(1,1,1);
    sprintf(posText, "%x, %x", xpos, ypos);
    font.text(8, 720, posText);
    if(mousex < 960)
      sprintf(posText, "%i, %i", mousex/12, mousey/12);
    font.text(8, 700, posText);

    SDL_GL_SwapBuffers();
    stop = clock();
    if(stop-start < FRAMERATE)
      SDL_Delay(FRAMERATE - (stop-start));
    }
}

void glInit(short w, short h)
{

  SDL_Init(SDL_INIT_VIDEO);  
  SDL_SetVideoMode(w, h, 0, SDL_OPENGL);//|SDL_FULLSCREEN);

  glViewport(0, 0, w, h);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_FLAT); 

  glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, w, 0, h);

  glMatrixMode(GL_MODELVIEW);

  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

}
short checkPoint(short xpos, short ypos, short left, short top, short w, short h)
{
  if(xpos > left && xpos < left+w)
    if(ypos > top && ypos < top+h)
      return 1;
  return 0;
}
