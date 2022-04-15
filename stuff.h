#ifndef STUFF_H
#define STUFF_H

class stuff
{
public:

  void initialize();
  void save();
  void clear();

  short checkPowerup(purist* dood, short roomX, short roomY);
  void forceDraw(short roomX, short roomY);
  void drawPowerups(short roomX, short roomY);

  void addPowerup(short type, short xpos, short ypos, short roomx, short roomy);

  void erase(short roomx, short roomy, short xpos, short ypos);

private:

  short jump1[15][5];
  short jump2[15][5];
  short health[30][5];
  short all[3][5];

  short bosses[4];

};

// powerup second index:
// 0 - taken
// 1 - roomx
// 2 - roomy
// 3 - xpos
// 4 - ypos

// saves second index:
// 0 - roomx
// 1 - roomy
// 2 - xpos (halfgrid)
// 3 - ypos (halfgrid)
#endif
