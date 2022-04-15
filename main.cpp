#include <cstdlib> 
#include <iostream>
#include <cmath>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <ik/irrKlang.h>
#include "event.h"
#include "sfont.h"
#include "room.h"
#include "explosion.h"

using namespace std;
using namespace irrklang;

#define SCROLL 32.0
#define SLEFT player.xpos < SCROLL && player.roomX > 0 && !rooms[player.roomX][player.roomY].hasABoss()
#define SRIGHT player.xpos > 320-SCROLL && player.roomX < 14 && !rooms[player.roomX][player.roomY].hasABoss()
#define SUP player.ypos < SCROLL && player.roomY > 0 && !rooms[player.roomX][player.roomY].hasABoss()
#define SDOWN player.ypos > 240-SCROLL && player.roomY < 12 && !rooms[player.roomX][player.roomY].hasABoss()
#define FBOSS(x, y) (x==7 && y==7)

#define FRAMERATE 16

void glInit(short w, short h);

int main(int argc, char *argv[])
{
  putenv("SDL_VIDEO_WINDOW_POS=38,38");

  long e,x,i,y;
  long start, stop;
  short power;
  e=0;

  short phase = 0;
  short selection = 0;
  short warpX, warpY;

  short flashCounter = 0;
  short lock = 0;

  double rnd;

  GLuint splash1, splash2;

  char numString[256];

  short deadTemp = 0;
  short refX = 0;
  short refY = 0;

  /*******************/
  /* Sound Variables */
  /*******************/

  ISoundEngine* engine;  

  ISoundSource* music;
  ISoundSource* jump1;
  ISoundSource* jump2;
  ISoundSource* jump3;
  ISoundSource* jumpX;
  ISoundSource* save;
  ISoundSource* jup1;
  ISoundSource* jup2;
  ISoundSource* health;
  ISoundSource* all;
  ISoundSource* pain;
  ISoundSource* charge;
  ISoundSource* bossPain;
  ISoundSource* boom;
  ISoundSource* lastboom;

  float volume = 1;

  float wid = 1;
  float hei = 1;

  bool isPlaying;                                              //IsPlaying Variable

  short kState = 0;
  short kOn = 0;


  /**************/
  /* Audio Init */
  /**************/

  engine = createIrrKlangDevice();

  music = engine->addSoundSourceFromFile("wav/");
  jump1 = engine->addSoundSourceFromFile("wav/jump1b.ogg");
  jump2 = engine->addSoundSourceFromFile("wav/jump2b.ogg");
  jump3 = engine->addSoundSourceFromFile("wav/jump3.ogg");
  jumpX = engine->addSoundSourceFromFile("wav/jumpxb.ogg");
  save  = engine->addSoundSourceFromFile("wav/save.ogg");
  jup1  = engine->addSoundSourceFromFile("wav/jup1.wav");
  jup2  = engine->addSoundSourceFromFile("wav/jup2.wav");
  health = engine->addSoundSourceFromFile("wav/health.wav");
  all   = engine->addSoundSourceFromFile("wav/all.wav");
  pain  = engine->addSoundSourceFromFile("wav/pain.wav");
  charge = engine->addSoundSourceFromFile("wav/charge.ogg");
    charge->setDefaultVolume(.5);
  bossPain  = engine->addSoundSourceFromFile("wav/bosspain.wav");
  boom  = engine->addSoundSourceFromFile("wav/bubble3.wav");
  lastboom  = engine->addSoundSourceFromFile("wav/lastboom.wav");

  engine->setSoundVolume(volume);

  /******************/
  /* Graphical Init */
  /******************/

  glInit(969,720);

  SDL_Surface *icon = SDL_LoadBMP("data/icon.bmp");

  SDL_WM_SetIcon(icon, NULL);
  SDL_WM_SetCaption("Arity 1", "Arity 1");

  sfont font;
  font.init();
  font.load("data/SLGFont2.srf");

  SDL_Event event;
  Uint8* keys;

  gameEvents.init();

  list explosions;
  explosions.initialize();
  Explosion* tempExp;

  purist player;
  player.initialize();
  player.load();

  room** rooms;
  rooms = new room*[15];
  for(x = 0; x < 15; ++x)
    rooms[x] = new room[13];

  for(x = 0; x < 15; ++x)
    for(y = 0; y < 13; ++y)
    {
      rooms[x][y].initialize();
      rooms[x][y].setBosses(&player);
      rooms[x][y].load(x, y);
    }

  stuff powerups;
  powerups.initialize();

  while(e==0)
  {
    start = SDL_GetTicks();
    keys=SDL_GetKeyState(NULL);
    SDL_PollEvent(&event);
    if(keys[SDLK_ESCAPE])
      switch(phase)
      {
      case 0:
        kState = 0;
        e=1;
      break;
      case 1:
        keys[SDLK_ESCAPE] = 0;
        if(!flashCounter && !lock)
          phase = 0;
      break;
      case 2:
        keys[SDLK_ESCAPE] = 0;
        phase = 1;
      break;
      }

    if(phase == 0)
    {
      if(keys[SDLK_UP])
      {
        if(kState < 2)
          ++kState;
        else
          kState = 0;
        keys[SDLK_UP] = 0;
        if(selection > 0)
          --selection;
      }
      if(keys[SDLK_DOWN])
      {
        if(kState == 2 || kState == 3)
          ++kState;
        else
          kState = 0;
        keys[SDLK_DOWN] = 0;
        if(selection < 3)
          ++selection;
      }
      if(keys[SDLK_LEFT])
      {
        keys[SDLK_LEFT] = 0;
        if(kState == 4 || kState == 6)
          ++kState;
        else
          kState = 0;
      }
      if(keys[SDLK_RIGHT])
      {
        keys[SDLK_RIGHT] = 0;
        if(kState == 5 || kState == 7)
          ++kState;
        else
          kState = 0;
      }
      if(keys[SDLK_z])
      {
        keys[SDLK_z] = 0;
        if(kState == 8)
          ++kState;
        else
          kState = 0;
      }
      if(keys[SDLK_x])
      {
        keys[SDLK_x] = 0;
        if(kState == 9)
        {
          ++kOn;
          kOn%=2;
          kState = 0;
          selection = 0;
        }
      }
      if(keys[SDLK_SPACE] || keys[SDLK_RETURN])
      {
        kState = 0;
        keys[SDLK_SPACE] = 0;
        keys[SDLK_RETURN] = 0;
        switch(selection)
        {
          case 0:
            phase = 1;
          break;
          case 1:
            player.load();
            powerups.initialize();
            for(x = 0; x < 15; ++x)
              for(y = 0; y < 13; ++y)
                rooms[x][y].reset();
            phase = 1;
            selection = 0;
          break;
          case 2:
            player.initialize();
            powerups.initialize();
            powerups.clear();
            for(x = 0; x < 15; ++x)
              for(y = 0; y < 13; ++y)
              {
                rooms[x][y].initialize();
                rooms[x][y].setBosses(&player, 1);
                rooms[x][y].load(x, y);
              }
            phase = 1;
            selection = 0;
          break;
          case 3:
            e = 1;
          break;
        }
      }
    }
    if(phase == 1)
    {
      if(keys[SDLK_r])
      {
        keys[SDLK_r] = 0;
        player.load();
        powerups.initialize();
        for(x = 0; x < 15; ++x)
          for(y = 0; y < 13; ++y)
            rooms[x][y].reset();
      }

      if(keys[SDLK_LEFT])
      {
        if(player.xvel > 0)
          player.xvel = 0;
        else if(player.xvel > -1-kOn)
          player.xvel-=.25;
      }
      if(keys[SDLK_RIGHT])
      {
        if(player.xvel < 0)
          player.xvel = 0;
        else if(player.xvel < 1+kOn)
          player.xvel+=.25;
      }
      if(!(keys[SDLK_RIGHT] || keys[SDLK_LEFT]))
      {
        if(player.xvel < .5 && player.xvel > -.5)
          player.xvel = 0;
        if(player.xvel <= -.5)
          player.xvel+=.5;
        if(player.xvel >= .5)
          player.xvel-=.5;
      }
      if(keys[SDLK_UP] && (player.onGround || kOn))
      {
        keys[SDLK_UP] = 0;
        if(rooms[player.roomX][player.roomY].playerSave(&player, &powerups) || kOn)
        {
          engine->play2D(save);       
          player.health = player.maxHealth;
          keys[SDLK_DOWN]=0;
          phase = 2;
          warpX = player.roomX;
          warpY = player.roomY;
        }
      }
      if(keys[SDLK_DOWN] && player.onGround)
      {
        if(rooms[player.roomX][player.roomY].playerSave(&player, &powerups))
        {
          engine->play2D(save);    
          player.health = player.maxHealth;
          keys[SDLK_DOWN]=0;
        }
      }
      if(keys[SDLK_SPACE] && player.jumps == 0)
      {
        engine->play2D(jump1);
        player.yvel = player.jump1;
        ++player.jumps;
        keys[SDLK_SPACE] = 0;
      }
      if(keys[SDLK_SPACE] && player.jumps == 1)
      {
        engine->play2D(jump2);
        player.yvel = player.jump2;
        ++player.jumps;
        keys[SDLK_SPACE] = 0;
      }
      if(keys[SDLK_SPACE] && player.jumps == 2)
      {
        if(player.yvel < .15 && player.yvel > -.15)
        {
          engine->play2D(jump3);
          player.yvel = -1.57;
        }
        else
        {
          engine->play2D(jumpX);
        }
        ++player.jumps;
        keys[SDLK_SPACE] = 0;
      }
      if(kOn)
        player.jumps = 0;

      if(player.yvel < 3)
        player.yvel +=.1;

      rooms[player.roomX][player.roomY].collide(&player);

      switch(powerups.checkPowerup(&player, player.roomX, player.roomY))
      {
      case 1:
        engine->play2D(jup1);
      break;
      case 2:
        engine->play2D(jup2);
      break;
      case 3:
        engine->play2D(health);
      break; 
      case 4:
        engine->play2D(all);
      break; 
      }

      for(x = 0; x < 15; ++x)
        for(y = 0; y < 13; ++y)
          rooms[x][y].update();

      explosions.setPointer(0);
      if(explosions.getLength()) do
      {
        tempExp = (Explosion*)explosions.getPointer();
        if(tempExp->update())
        {
          explosions.delPointer();
        }
      } while(!explosions.incPointer());

    }
    if(phase == 2)
    {
      if(keys[SDLK_LEFT] && warpX > 0)
      {
        keys[SDLK_LEFT] = 0;
        --warpX;
      }
      if(keys[SDLK_RIGHT] && warpX < 14)
      {
        keys[SDLK_RIGHT] = 0;
        ++warpX;
      }
      if(keys[SDLK_UP] && warpY > 0)
      {
        keys[SDLK_UP] = 0;
        --warpY;
      }
      if(keys[SDLK_DOWN] && warpY < 12)
      {
        keys[SDLK_DOWN] = 0;
        ++warpY;
      }
      if((keys[SDLK_SPACE] || keys[SDLK_RETURN]) && (player.visited[warpX][warpY] == 2 || kOn))
      {
        if(rooms[warpX][warpY].warp(&player))
        {
          engine->play2D(save);  
          phase = 1;
        }
        keys[SDLK_RETURN] = 0;
        keys[SDLK_SPACE] = 0;
      }

    }

    if(flashCounter)
    {
      --flashCounter;
      if(!flashCounter)
        lock = 0;
    }
   

    while(gameEvents.hasEvent())
    {
      switch(gameEvents.getEvent())
      {
      case BOSSDEAD:
        for(refX; refX < 15; ++refX)
          for(refY; refY < 13; ++refY)
          {
            rooms[refX][refY].setBosses(&player, 1);
            rooms[refX][refY].load(refX, refY);
          }
          player.save(player.roomX, player.roomY);
          rooms[player.roomX][player.roomY].setBosses(&player, 1);
         refX = 0;
         refY = 0;
         flashCounter = 100;
         engine->play2D(lastboom);
      break;
      case DEATHROLL:
        lock = 1;
        if(refX == player.roomX && refY == player.roomY)
          ++refY;
        if(refX < 15)
        {
          rooms[refX][refY].setBosses(&player, 1);
          rooms[refX][refY].load(refX, refY);
        }
        ++refY;
        if(refY == 12)
        {
          refY = 0;
          ++refX;
        }
        tempExp = new Explosion(260+rand()%440, 696-rand()%264, 12);
        explosions.add(tempExp);
        if(!engine->isCurrentlyPlaying(boom))
          engine->play2D(boom);
      break;
      case PLAYERHURT:
        if(!engine->isCurrentlyPlaying(pain))
          engine->play2D(pain);
      break;
      case PLAYERCHARGE:
        if(!engine->isCurrentlyPlaying(charge))
          engine->play2D(charge);
      break;
      case BOSSHURT:
        if(!engine->isCurrentlyPlaying(bossPain))
          engine->play2D(bossPain);
      break;
      }
    }

    if(player.health < 0)
    {
      tempExp = new Explosion(3*player.xpos, 720-3*player.ypos, 12);
      explosions.add(tempExp);
      player.load();
      powerups.initialize();
      for(x = 0; x < 15; ++x)
        for(y = 0; y < 13; ++y)
          rooms[x][y].reset();
    }

    /***************/
    /* Sound Phase */
    /***************/

    /******************/
    /* Graphics Phase */
    /******************/

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);
    glLoadIdentity();			         	// Reset The View
    glEnable(GL_BLEND);

    glTranslated(9,0,0);

    //All this SLEFT crap is the smoooooth scrolling code that I hacked in at the last minute.

    if(SLEFT)
      glTranslated(-480*((player.xpos-SCROLL)/SCROLL), 0, 0);
    if(SRIGHT)
      glTranslated(480*((320-player.xpos-SCROLL)/SCROLL), 0, 0);
    if(SUP)
      glTranslated(0,360*((player.ypos-SCROLL)/SCROLL), 0);
    if(SDOWN)
      glTranslated(0,-360*((240-player.ypos-SCROLL)/SCROLL), 0); 

    rooms[player.roomX][player.roomY].draw();
    powerups.drawPowerups(player.roomX, player.roomY);

    if(SLEFT)
    {
      glTranslated(-960, 0, 0);
      if(!FBOSS(player.roomX-1,player.roomY))
      {
        rooms[player.roomX-1][player.roomY].draw();
        powerups.drawPowerups(player.roomX-1, player.roomY);
      }
        if(SUP && !FBOSS(player.roomX-1,player.roomY-1))
        {
          glTranslated(0, 720, 0);
          rooms[player.roomX-1][player.roomY-1].draw();
          powerups.drawPowerups(player.roomX-1, player.roomY-1);
          glTranslated(0, -720, 0);
        }
        if(SDOWN && !FBOSS(player.roomX-1,player.roomY+1))
        {
          glTranslated(0, -720, 0);
          rooms[player.roomX-1][player.roomY+1].draw();
          powerups.drawPowerups(player.roomX-1, player.roomY+1);
          glTranslated(0, 720, 0);
        }
      glTranslated(960, 0, 0);
    }
    if(SRIGHT)
    {
      glTranslated(960, 0, 0);
      if(!FBOSS(player.roomX+1,player.roomY))
      {
        rooms[player.roomX+1][player.roomY].draw();
        powerups.drawPowerups(player.roomX+1, player.roomY);
      }
        if(SUP && !FBOSS(player.roomX+1,player.roomY-1))
        {
          glTranslated(0, 720, 0);
          rooms[player.roomX+1][player.roomY-1].draw();
          powerups.drawPowerups(player.roomX+1, player.roomY-1);
          glTranslated(0, -720, 0);
        }
        if(SDOWN && !FBOSS(player.roomX+1,player.roomY+1))
        {
          glTranslated(0, -720, 0);
          rooms[player.roomX+1][player.roomY+1].draw();
          powerups.drawPowerups(player.roomX+1, player.roomY+1);
          glTranslated(0, 720, 0);
        }
      glTranslated(-960, 0, 0);
    }
    if(SUP && !FBOSS(player.roomX,player.roomY-1))
    {
      glTranslated(0, 720, 0);
      rooms[player.roomX][player.roomY-1].draw();
      powerups.drawPowerups(player.roomX, player.roomY-1);
      glTranslated(0, -720, 0);
    }
    if(SDOWN && !FBOSS(player.roomX,player.roomY+1))
    {
      glTranslated(0, -720, 0);
      rooms[player.roomX][player.roomY+1].draw();
      powerups.drawPowerups(player.roomX, player.roomY+1);
      glTranslated(0, 720, 0);
    }

    explosions.setPointer(0);
    if(explosions.getLength()) do
    {
      tempExp = (Explosion*)explosions.getPointer();
      tempExp->draw();
    } while(!explosions.incPointer());

    player.draw();

    if(SDOWN)
      glTranslated(0,360*((240-player.ypos-SCROLL)/SCROLL), 0); 
    if(SUP)
      glTranslated(0,-360*((player.ypos-SCROLL)/SCROLL), 0);
    if(SRIGHT)
      glTranslated(-480*((320-player.xpos-SCROLL)/SCROLL), 0, 0);
    if(SLEFT)
      glTranslated(480*((player.xpos-SCROLL)/SCROLL), 0, 0);

    if(flashCounter)
    {
      glColor4f(1,1,1,flashCounter/100.0);
      glBegin(GL_QUADS);
        glVertex2d(0,0);
        glVertex2d(960,0);
        glVertex2d(960,720);
        glVertex2d(0,720);
      glEnd();
    }

    glTranslated(-9,0,0);

    glColor3f(.5,.5,.5); 
    glBegin(GL_LINES);
      glVertex2d(9, 0);
      glVertex2d(9, 720);
    glEnd();

    glColor3f(1,0,0); 
    glBegin(GL_QUADS);
      glVertex2d(3, 0);
      glVertex2d(3, player.health*3);
      glVertex2d(6, player.health*3);
      glVertex2d(6, 0);
    glEnd();

    glColor3f(.5,0,0); 
    glBegin(GL_QUADS);
      glVertex2d(3, 0);
      glVertex2d(3, player.maxHealth*3);
      glVertex2d(6, player.maxHealth*3);
      glVertex2d(6, 0);
    glEnd();

    glColor3f(0,0,0); 
    glBegin(GL_QUADS);
      glVertex2d(0, 0);
      glVertex2d(0, 720);
      glVertex2d(9, 720);
      glVertex2d(9, 0);
    glEnd();


    glClear(GL_DEPTH_BUFFER_BIT);

    glTranslated(9,0,0);

    if(phase == 0)
    {

      glColor4f(0,0,0,.5);
      glBegin(GL_QUADS);
        glVertex2d(0,0);
        glVertex2d(0,720);
        glVertex2d(960,720);
        glVertex2d(960,0);
      glEnd();
  
      glClear(GL_DEPTH_BUFFER_BIT);

      for(x = 0; x < 15; ++x)
        for(y = 0; y < 13; ++y)
        {
          if(player.visited[x][y] || kOn)
          {
            rooms[x][y].drawMap(180+x*40, 165+(12-y)*30);
          }
          else
          {
            rnd = rand()/(double)RAND_MAX;
            glColor3f(rnd,rnd,rnd);
            glBegin(GL_QUADS);
              glVertex2d(180+x*40, 165+(12-y)*30);
              glVertex2d(180+x*40, 165+(12-y)*30+30);
              glVertex2d(180+x*40+40, 165+(12-y)*30+30);
              glVertex2d(180+x*40+40, 165+(12-y)*30);
            glEnd();
          }
        }

      glColor4f(.5,.5,.5,1);
      glBegin(GL_LINE_LOOP);
        glVertex2d(179, 165);
        glVertex2d(180, 555);
        glVertex2d(781, 555);
        glVertex2d(781, 165);
      glEnd();

      glColor4f(1,1,0,1);
      glBegin(GL_LINE_LOOP);
        glVertex2d(179+player.roomX*40, 165+(12-player.roomY)*30);
        glVertex2d(180+player.roomX*40, 165+(12-player.roomY)*30+30);
        glVertex2d(180+player.roomX*40+40, 165+(12-player.roomY)*30+30);
        glVertex2d(180+player.roomX*40+40, 165+(12-player.roomY)*30);
      glEnd();

      glColor4f(1,1,1,1);
      sprintf(numString, "Arity %i", 4-(player.bosses[0]+player.bosses[1]+player.bosses[2]+player.bosses[3]));
      font.text(16,720-16,numString,-1, 6);
      glColor4f(.5,.5,.5,1);
      if(selection == 0)   
        glColor4f(1,1,1,1);
      font.text(16,720-128,"Continue",-1, 2);
      glColor4f(.5,.5,.5,1);
      if(selection == 1)   
        glColor4f(1,1,1,1);
      font.text(16,720-160,"Reload",-1, 2);
      glColor4f(.5,.5,.5,1);
      if(selection == 2)   
        glColor4f(1,1,1,1);
      font.text(16,720-192,"New",-1, 2);
      glColor4f(.5,.5,.5,1);
      if(selection == 3)   
        glColor4f(1,1,1,1);
      font.text(16,720-224,"Exit",-1, 2);
      glColor4f(1,1,1,1);
      sprintf(numString, "%3.1f%%", 100*(.25*(-(2+player.jump1))/2.7 + .25*(-(1.5+player.jump2))/2.1 + .5*((player.maxHealth-60))/180.0)); //2/5.1, 1.5/3.8, 60/240
      font.text(916,718,numString,-1,1);

    }
    if(phase == 2)
    {
      for(x = 0; x < 15; ++x)
        for(y = 0; y < 13; ++y)
        {
          if(player.visited[x][y] || kOn)
          {
            rooms[x][y].drawMap(180+x*40, 165+(12-y)*30);
          }
          else
          {
            rnd = rand()/(double)RAND_MAX;
            glColor3f(rnd,rnd,rnd);
            glBegin(GL_QUADS);
              glVertex2d(180+x*40, 165+(12-y)*30);
              glVertex2d(180+x*40, 165+(12-y)*30+30);
              glVertex2d(180+x*40+40, 165+(12-y)*30+30);
              glVertex2d(180+x*40+40, 165+(12-y)*30);
            glEnd();
          }
        }

      glColor4f(.5,.5,.5,1);
      glBegin(GL_LINE_LOOP);
        glVertex2d(179, 165);
        glVertex2d(180, 555);
        glVertex2d(781, 555);
        glVertex2d(781, 165);
      glEnd();

      glColor4f(1,1,0,1);
      glBegin(GL_LINE_LOOP);
        glVertex2d(179+player.roomX*40, 165+(12-player.roomY)*30);
        glVertex2d(180+player.roomX*40, 165+(12-player.roomY)*30+30);
        glVertex2d(180+player.roomX*40+40, 165+(12-player.roomY)*30+30);
        glVertex2d(180+player.roomX*40+40, 165+(12-player.roomY)*30);
      glEnd();
      glBegin(GL_LINES);
        glVertex2d(180+warpX*40+10, 165+(12-warpY)*30+15);
        glVertex2d(180+warpX*40+30, 165+(12-warpY)*30+15);
        glVertex2d(180+warpX*40+20, 165+(12-warpY)*30+5);
        glVertex2d(180+warpX*40+20, 165+(12-warpY)*30+25);
      glEnd();
      if(player.visited[warpX][warpY] == 2)
      {
        glBegin(GL_LINES);
          glVertex2d(180+warpX*40+15, 165+(12-warpY)*30+10);
          glVertex2d(180+warpX*40+25, 165+(12-warpY)*30+20);
          glVertex2d(180+warpX*40+25, 165+(12-warpY)*30+10);
          glVertex2d(180+warpX*40+15, 165+(12-warpY)*30+20);
        glEnd();
      }

    }

    if(kOn)
    {
      glColor4f(1,1,0,1);
      font.text(16,720,"KONAMI CODE IS ACTIVATE",-1, 1);
    }

    SDL_GL_SwapBuffers();
    stop = SDL_GetTicks();
    if(stop-start < FRAMERATE)
      SDL_Delay(FRAMERATE - (stop-start));
    }
  engine->drop();
  return 0;
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

  gluOrtho2D(0, 969, 0, 720);

  glMatrixMode(GL_MODELVIEW);

  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

}

