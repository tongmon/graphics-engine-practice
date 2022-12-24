#ifndef __SPLINEACTOR_H__
#define __SPLINEACTOR_H__

#include "Actor.h"

class SplineActor : public Actor {
  public:
    SplineActor(class Game *game);

    void ActorInput(const uint8_t *keys) override;

    void RestartSpline();

  private:
    class SplineCamera *mCameraComp;
};

#endif // __SPLINEACTOR_H__