#ifndef __FOLLOWACTOR_H__
#define __FOLLOWACTOR_H__

#include "Actor.h"

class FollowActor : public Actor {
  public:
    FollowActor(class Game *game);

    void ActorInput(const uint8_t *keys) override;

    void SetVisible(bool visible);

  private:
    class MoveComponent *mMoveComp;
    class FollowCamera *mCameraComp;
    class MeshComponent *mMeshComp;
};

#endif // __FOLLOWACTOR_H__