#ifndef __SKELETALMESHCOMPONENT_H__
#define __SKELETALMESHCOMPONENT_H__

#include "MatrixPalette.h"
#include "MeshComponent.h"

class SkeletalMeshComponent : public MeshComponent {
  public:
    SkeletalMeshComponent(class Actor *owner);
    // Draw this mesh component
    void Draw(class Shader *shader) override;

    void Update(float deltaTime) override;

    // Setters
    void SetSkeleton(const class Skeleton *sk) { mSkeleton = sk; }

    // Play an animation. Returns the length of the animation
    float PlayAnimation(const class Animation *anim, float playRate = 1.0f);

  protected:
    void ComputeMatrixPalette();

    MatrixPalette mPalette;
    const class Skeleton *mSkeleton;
    const class Animation *mAnimation;
    float mAnimPlayRate;
    float mAnimTime;
};

#endif // __SKELETALMESHCOMPONENT_H__