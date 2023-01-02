#ifndef __ORBITACTOR_H__
#define __ORBITACTOR_H__

#include "Actor.h"

class OrbitActor : public Actor {
  public:
    OrbitActor(class Game *game);

    void ActorInput(const uint8_t *keys) override;

    void SetVisible(bool visible);

  private:
    class OrbitCamera *mCameraComp;
    class MeshComponent *mMeshComp;
};

#endif // __ORBITACTOR_H__