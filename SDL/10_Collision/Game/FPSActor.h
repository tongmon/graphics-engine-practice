#ifndef __FPSACTOR_H__
#define __FPSACTOR_H__

#include "Actor.h"
#include "SoundEvent.h"

class FPSActor : public Actor {
  public:
    FPSActor(class Game *game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keys) override;

    void Shoot();

    void SetFootstepSurface(float value);

    void SetVisible(bool visible);

    void FixCollisions();

  private:
    class MoveComponent *mMoveComp;
    class AudioComponent *mAudioComp;
    class MeshComponent *mMeshComp;
    class FPSCamera *mCameraComp;
    class BoxComponent *mBoxComp;
    class Actor *mFPSModel;
    SoundEvent mFootstep;
    float mLastFootstep;
};

#endif // __FPSACTOR_H__