#ifndef __BALLACTOR_H__
#define __BALLACTOR_H__

#include "Actor.h"

class BallActor : public Actor {
  public:
    BallActor(class Game *game);

    void UpdateActor(float deltaTime) override;

    void HitTarget();

    void LoadProperties(const rapidjson::Value &inObj) override;
    void SaveProperties(rapidjson::Document::AllocatorType &alloc,
                        rapidjson::Value &inObj) const override;

    TypeID GetType() const override { return TBallActor; }

  private:
    class AudioComponent *mAudioComp;
    float mLifeSpan;
};

#endif // __BALLACTOR_H__