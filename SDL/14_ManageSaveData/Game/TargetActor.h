#ifndef __TARGETACTOR_H__
#define __TARGETACTOR_H__

#include "Actor.h"

class TargetActor : public Actor {
  public:
    TargetActor(class Game *game);
    TypeID GetType() const override { return TTargetActor; }
};

#endif // __TARGETACTOR_H__