#ifndef __TARGETCOMPONENT_H__
#define __TARGETCOMPONENT_H__

#include "Component.h"

class TargetComponent : public Component {
  public:
    TargetComponent(class Actor *owner);
    ~TargetComponent();
    TypeID GetType() const override { return TTargetComponent; }
};

#endif // __TARGETCOMPONENT_H__