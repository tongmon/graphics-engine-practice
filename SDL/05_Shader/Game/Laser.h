#ifndef __LASER_H__
#define __LASER_H__

#include "Actor.h"

class Laser : public Actor {
  public:
    Laser(class Game *game);

    void UpdateActor(float deltaTime) override;

  private:
    class CircleComponent *mCircle;
    float mDeathTimer;
};

#endif // __LASER_H__