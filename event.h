#ifndef EVENT_H
#define EVENT_H

#include "list.h"

#define BOSSDEAD 1
#define DEATHROLL 2
#define PLAYERHURT 3
#define PLAYERCHARGE 4
#define BOSSHURT 5

class GameEvents
{
public:

  void init();
  void addEvent(short event);
  short getEvent();
  short hasEvent();

private:

  list events;

};

extern GameEvents gameEvents;

#endif
