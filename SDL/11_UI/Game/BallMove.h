#ifndef __BALLMOVE_H__
#define __BALLMOVE_H__

#include "MoveComponent.h"

class BallMove : public MoveComponent {
  public:
    BallMove(class Actor *owner);

    void SetPlayer(Actor *player) { mPlayer = player; }
    void Update(float deltaTime) override;

  protected:
    class Actor *mPlayer;
};
#endif // __BALLMOVE_H__