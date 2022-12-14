#ifndef __SPRITECOMPONENT_H__
#define __SPRITECOMPONENT_H__

#include "Component.h"
#include <SDL2/SDL.h>

class SpriteComponent : public Component {
  public:
    // (Lower draw order corresponds with further back)
    SpriteComponent(class Actor *owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void Draw(class Shader *shader);
    virtual void SetTexture(class Texture *texture);

    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexHeight() const { return mTexHeight; }
    int GetTexWidth() const { return mTexWidth; }

  protected:
    class Texture *mTexture;
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;
};

#endif // __SPRITECOMPONENT_H__