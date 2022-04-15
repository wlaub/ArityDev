#ifndef BUTTON_H
#define BUTTON_H

class button
{
public:
  void initialize(short xpos, short ypos, char* filename);
  void initialize(short xpos, short ypos);
  void draw(SDL_Surface* s);
  void draw(SDL_Surface* s, short r, short g, short b);
  void toggleActive();
  void setActive();
  void setInactive();
  short getActive();
  short getCollision(short xpos, short ypos);
  void collisionActive(short xpos, short ypos);

private:
  SDL_Rect img;
  SDL_Rect target;
  SDL_Surface* image;
  short active;
  short over;

};

#endif
