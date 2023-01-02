#ifndef __BALLMOVE_H__
#define __BALLMOVE_H__

#include "MoveComponent.h"

class BallMove : public MoveComponent {
  public:
    BallMove(class Actor *owner);

    void Update(float deltaTime) override;

    TypeID GetType() const override { return TBallMove; }

  protected:
};
#endif // __BALLMOVE_H__