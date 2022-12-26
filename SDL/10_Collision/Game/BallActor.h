#ifndef __BALLACTOR_H__
#define __BALLACTOR_H__

#include "Actor.h"

class BallActor : public Actor {
  public:
    BallActor(class Game *game);

    void UpdateActor(float deltaTime) override;
    void SetPlayer(Actor *player);

    void HitTarget();

  private:
    class AudioComponent *mAudioComp;
    class BallMove *mMyMove;
    float mLifeSpan;
};

#endif // __BALLACTOR_H__