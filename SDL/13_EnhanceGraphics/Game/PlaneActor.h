#ifndef __PLANEACTOR_H__
#define __PLANEACTOR_H__

#include "Actor.h"

class PlaneActor : public Actor {
  public:
    PlaneActor(class Game *game);
    ~PlaneActor();
    class BoxComponent *GetBox() { return mBox; }

  private:
    class BoxComponent *mBox;
};

#endif // __PLANEACTOR_H__