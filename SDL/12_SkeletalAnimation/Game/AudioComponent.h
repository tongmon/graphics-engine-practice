#ifndef __AUDIOCOMPONENT_H__
#define __AUDIOCOMPONENT_H__

#include "Component.h"
#include "SoundEvent.h"
#include <string>
#include <vector>

class AudioComponent : public Component {
  public:
    AudioComponent(class Actor *owner, int updateOrder = 200);
    ~AudioComponent();

    void Update(float deltaTime) override;
    void OnUpdateWorldTransform() override;

    SoundEvent PlayEvent(const std::string &name);
    void StopAllEvents();

  private:
    std::vector<SoundEvent> mEvents2D;
    std::vector<SoundEvent> mEvents3D;
};
#endif // __AUDIOCOMPONENT_H__