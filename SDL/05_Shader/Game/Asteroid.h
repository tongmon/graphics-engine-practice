#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "Actor.h"

class Asteroid : public Actor {
  public:
    Asteroid(class Game *game);
    ~Asteroid();

    class CircleComponent *GetCircle() { return mCircle; }

  private:
    class CircleComponent *mCircle;
};

#endif // __ASTEROID_H__