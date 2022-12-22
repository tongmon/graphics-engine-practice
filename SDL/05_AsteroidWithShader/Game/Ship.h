#ifndef __SHIP_H__
#define __SHIP_H__

#include "Actor.h"
class Ship : public Actor {
  public:
    Ship(class Game *game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keyState) override;

  private:
    float mLaserCooldown;
};
#endif // __SHIP_H__