#ifndef __MESHCOMPONENT_H__
#define __MESHCOMPONENT_H__

#include "Component.h"

class MeshComponent : public Component {
  public:
    MeshComponent(class Actor *owner, bool isSkeletal = false);
    ~MeshComponent();
    // Draw this mesh component
    virtual void Draw(class Shader *shader);
    // Set the mesh/texture index used by mesh component
    virtual void SetMesh(class Mesh *mesh) { mMesh = mesh; }
    void SetTextureIndex(size_t index) { mTextureIndex = index; }

    void SetVisible(bool visible) { mVisible = visible; }
    bool GetVisible() const { return mVisible; }

    bool GetIsSkeletal() const { return mIsSkeletal; }

    TypeID GetType() const override { return TMeshComponent; }

    void LoadProperties(const rapidjson::Value &inObj) override;
    void SaveProperties(rapidjson::Document::AllocatorType &alloc,
                        rapidjson::Value &inObj) const override;

  protected:
    class Mesh *mMesh;
    size_t mTextureIndex;
    bool mVisible;
    bool mIsSkeletal;
};
#endif // __MESHCOMPONENT_H__