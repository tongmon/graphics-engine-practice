#ifndef __TARGETCOMPONENT_H__
#define __TARGETCOMPONENT_H__

#include "Component.h"

class TargetComponent : public Component {
  public:
    TargetComponent(class Actor *owner);
    ~TargetComponent();
};

#endif // __TARGETCOMPONENT_H__