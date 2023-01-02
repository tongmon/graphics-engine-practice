#ifndef __MATRIXPALETTE_H__
#define __MATRIXPALETTE_H__

#include "MyMath.h"

const size_t MAX_SKELETON_BONES = 96;

struct MatrixPalette {
    Matrix4 mEntry[MAX_SKELETON_BONES];
};

#endif // __MATRIXPALETTE_H__