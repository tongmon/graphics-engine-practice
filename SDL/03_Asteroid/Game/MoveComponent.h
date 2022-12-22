#ifndef __MOVECOMPONENT_H__
#define __MOVECOMPONENT_H__

#include "Component.h"

class MoveComponent : public Component {
  public:
    // Lower update order to update first
    MoveComponent(class Actor *owner, int updateOrder = 10);

    void Update(float deltaTime) override;

    float GetAngularSpeed() const {
        return mAngularSpeed;
    }
    float GetForwardSpeed() const {
        return mForwardSpeed;
    }
    void SetAngularSpeed(float speed) {
        mAngularSpeed = speed;
    }
    void SetForwardSpeed(float speed) {
        mForwardSpeed = speed;
    }

  private:
    // Controls rotation (radians/second)
    float mAngularSpeed;
    // Controls forward movement (units/second)
    float mForwardSpeed;
};

#endif // __MOVECOMPONENT_H__