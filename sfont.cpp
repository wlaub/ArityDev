#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "sfont.h"

/*************************************/
/* Second Law Games                  */
/*   ##### ##### ##### #   # #####   */
/*   #     #     #   # ##  #   #     */
/*   ##### ####  #   # # # #   #     */
/*       # #     #   # #  ##   #     */
/*   ##### #     ##### #   #   #     */
/*                            BETA 2 */
/*************************************/

using namespace std;

int sfont::init()
{
  short x, y;

  for(x = 0; x < 256; ++x)
    for(y = 0; y < 3; ++y)
      map[y][x] = 0;

  width = 0;
  height = 0;
  cHeight = 0;
  sheet = 0;
  spacing = 0;

  return 0;

}

int sfont::load(const char* filename)
{

  short number;
  unsigned char cChar;
  unsigned char *data;
  long i, j;

  char imgName[256];

  ifstream fin;

  fin.open(filename, ios_base::binary);

  width = fin.get();
  height = fin.get(); 
  spacing = fin.get(); 
  cHeight = fin.get();
  number = fin.get();

  i = 0;
  while(filename[i]!=0)
    ++i;
  if(filename[i-2] == 'r')
  {
    data = new unsigned char[width*height+width];

    for(i = 0; i < width*height+width; ++i)
      data[i] = 0;

    for(i = 0; i < number; ++i)
    {
      cChar = fin.get();
      map[0][cChar] = fin.get();
      map[1][cChar] = fin.get();
      map[2][cChar] = fin.get();
    }

    for(i = 0; i < width*height/8; ++i)
    {
      cChar = fin.get();
      for(j = 0; j < 8; ++j)
      {
        data[8*i+7-j] = ((cChar >> j)&0x01)*255;
      }
    }

    glGenTextures(1, &sheet);
    glBindTexture(GL_TEXTURE_2D, sheet);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }/*
  else if(filename[i-2] == 'p')
  {
    fin.getline(imgName, 256, 0);
    SDL_Surface* png = IMG_Load(imgName);

    glGenTextures(1, &sheet);
    glBindTexture(GL_TEXTURE_2D, sheet);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, png->w, png->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, png->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_FreeSurface(png);
  }*/

  fin.close();

  return 0;

}

int sfont::draw(double x, double y, unsigned char character)
{

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, sheet);
  glBegin(GL_QUADS);
    glTexCoord2d(map[0][character]/(double)width, 1-map[1][character]/(double)height);
      glVertex2d(x, y);
    glTexCoord2d(map[0][character]/(double)width, 1-(map[1][character]+cHeight)/(double)height);
      glVertex2d(x, y-cHeight);
    glTexCoord2d((map[0][character]+map[2][character])/(double)width, 1-(map[1][character]+cHeight)/(double)height);
      glVertex2d(x+map[2][character], y-cHeight);
    glTexCoord2d((map[0][character]+map[2][character])/(double)width, 1-map[1][character]/(double)height);
      glVertex2d(x+map[2][character], y);
  glEnd();

  glClear(GL_DEPTH_BUFFER_BIT);

  glDisable(GL_TEXTURE_2D);

  return 0;
}

int sfont::text(double x, double y, const char* text, long length, short scale)
{

  unsigned char character;
  long i = 0;

  double xpos = x;
  double ypos = y;

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, sheet);

  while(text[i] != 0 && (i < length || length == -1))
  {
    if(text[i] == 10)
    {
      xpos = x;
      ypos -= cHeight*scale;
    }
    else if(text[i] == '|')
    {
      xpos += map[2][0x20]*scale + spacing*scale;
    }
    else
    {
      character = text[i];
      glBegin(GL_QUADS);
        glTexCoord2d(map[0][character]/(double)width, 1-map[1][character]/(double)height);
          glVertex2d(xpos, ypos);
        glTexCoord2d(map[0][character]/(double)width, 1-(map[1][character]+cHeight)/(double)height);
          glVertex2d(xpos, ypos-cHeight*scale);
        glTexCoord2d((map[0][character]+map[2][character])/(double)width, 1-(map[1][character]+cHeight)/(double)height);
          glVertex2d(xpos+map[2][character]*scale, ypos-cHeight*scale);
        glTexCoord2d((map[0][character]+map[2][character])/(double)width, 1-map[1][character]/(double)height);
          glVertex2d(xpos+map[2][character]*scale, ypos);
      glEnd();
      xpos += map[2][character]*scale + spacing*scale;
    }
    ++i;
  }

  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_TEXTURE_2D);

  return 0;
}



