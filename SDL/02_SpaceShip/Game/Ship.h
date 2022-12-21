#ifndef __SHIP_H__
#define __SHIP_H__

#include "Actor.h"

class Ship : public Actor {
  public:
    Ship(class Game *game);
    void UpdateActor(float deltaTime) override;
    void ProcessKeyboard(const uint8_t *state);
    float GetRightSpeed() const { return mRightSpeed; }
    float GetDownSpeed() const { return mDownSpeed; }

  private:
    float mRightSpeed;
    float mDownSpeed;
};

#endif // __SHIP_H__