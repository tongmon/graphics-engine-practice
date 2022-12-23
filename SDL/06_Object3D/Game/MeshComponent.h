#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Component.h"
#include <cstddef>

class MeshComponent : public Component {
  public:
    MeshComponent(class Actor *owner);
    ~MeshComponent();
    // Draw this mesh component
    virtual void Draw(class Shader *shader);
    // Set the mesh/texture index used by mesh component
    virtual void SetMesh(class Mesh *mesh) { mMesh = mesh; }
    void SetTextureIndex(size_t index) { mTextureIndex = index; }

  protected:
    class Mesh *mMesh;
    size_t mTextureIndex;
};

#endif // __MESHCOMPONENT_H__