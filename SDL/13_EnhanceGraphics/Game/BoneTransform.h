#ifndef __BONETRANSFORM_H__
#define __BONETRANSFORM_H__

#include "MyMath.h"

class BoneTransform {
  public:
    // For now, just make this data public
    Quaternion mRotation;
    Vector3 mTranslation;

    Matrix4 ToMatrix() const;

    static BoneTransform Interpolate(const BoneTransform &a, const BoneTransform &b, float f);
};

#endif // __BONETRANSFORM_H__