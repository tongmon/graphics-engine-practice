#ifndef __FOLLOWACTOR_H__
#define __FOLLOWACTOR_H__

#include "Actor.h"

class FollowActor : public Actor {
  public:
    FollowActor(class Game *game);

    void ActorInput(const uint8_t *keys) override;

    void SetVisible(bool visible);

    void LoadProperties(const rapidjson::Value &inObj) override;
    void SaveProperties(rapidjson::Document::AllocatorType &alloc,
                        rapidjson::Value &inObj) const override;

    TypeID GetType() const override { return TFollowActor; }

  private:
    class MoveComponent *mMoveComp;
    class FollowCamera *mCameraComp;
    class SkeletalMeshComponent *mMeshComp;
    bool mMoving;
};

#endif // __FOLLOWACTOR_H__