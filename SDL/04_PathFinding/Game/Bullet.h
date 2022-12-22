#ifndef __BULLET_H__
#define __BULLET_H__

#include "Actor.h"

class Bullet : public Actor {
  public:
    Bullet(class Game *game);
    void UpdateActor(float deltaTime) override;

  private:
    class CircleComponent *mCircle;
    float mLiveTime;
};

#endif // __BULLET_H__