#ifndef __CAMERAACTOR_H__
#define __CAMERAACTOR_H__

#include "Actor.h"

class CameraActor : public Actor {
  public:
    CameraActor(class Game *game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keys) override;

  private:
    class MoveComponent *mMoveComp;
};

#endif // __CAMERAACTOR_H__