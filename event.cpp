#include "event.h"

void GameEvents::init()
{
  events.initialize();
}
void GameEvents::addEvent(short event)
{
  short* temp = new short;
  *temp = event;
  events.add(temp);
}
short GameEvents::getEvent()
{
  if(events.getLength() == 0)
    return 0;
  short* result;
  result = (short*)events.get(0);
  events.del(0);
  return *result;

}
short GameEvents::hasEvent()
{
  return events.getLength();
}

GameEvents gameEvents;
