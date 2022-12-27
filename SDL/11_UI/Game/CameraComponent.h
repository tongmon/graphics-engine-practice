#ifndef __CAMERACOMPONENT_H__
#define __CAMERACOMPONENT_H__

#include "Component.h"
#include "MyMath.h"

class CameraComponent : public Component {
  public:
    CameraComponent(class Actor *owner, int updateOrder = 200);

  protected:
    void SetViewMatrix(const Matrix4 &view);
};

#endif // __CAMERACOMPONENT_H__