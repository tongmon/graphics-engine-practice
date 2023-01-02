#ifndef __PLANEACTOR_H__
#define __PLANEACTOR_H__

#include "Actor.h"

class PlaneActor : public Actor {
  public:
    PlaneActor(class Game *game);
    TypeID GetType() const override { return TPlaneActor; }
};

#endif // __PLANEACTOR_H__