#ifndef __MIRRORCAMERA_H__
#define __MIRRORCAMERA_H__

#include "CameraComponent.h"

class MirrorCamera : public CameraComponent {
  public:
    MirrorCamera(class Actor *owner);

    void Update(float deltaTime) override;

    void SnapToIdeal();

    void SetHorzDist(float dist) { mHorzDist = dist; }
    void SetVertDist(float dist) { mVertDist = dist; }
    void SetTargetDist(float dist) { mTargetDist = dist; }

    TypeID GetType() const override { return TMirrorCamera; }

    void LoadProperties(const rapidjson::Value &inObj) override;
    void SaveProperties(rapidjson::Document::AllocatorType &alloc,
                        rapidjson::Value &inObj) const override;

  private:
    Vector3 ComputeCameraPos() const;

    // Horizontal follow distance
    float mHorzDist;
    // Vertical follow distance
    float mVertDist;
    // Target distance
    float mTargetDist;
};

#endif // __MIRRORCAMERA_H__