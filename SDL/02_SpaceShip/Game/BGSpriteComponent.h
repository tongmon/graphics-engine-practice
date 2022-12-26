#ifndef __BGSPRITECOMPONENT_H__
#define __BGSPRITECOMPONENT_H__

#include "SpriteComponent.h"
#include <MyMath.h>
#include <vector>

class BGSpriteComponent : public SpriteComponent {
  public:
    // Set draw order to default to lower (so it's in the background)
    BGSpriteComponent(class Actor *owner, int drawOrder = 10);
    // Update/draw overridden from parent
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer *renderer) override;
    // Set the textures used for the background
    void SetBGTextures(const std::vector<SDL_Texture *> &textures);
    // Get/set screen size and scroll speed
    void SetScreenSize(const Vector2 &size) { mScreenSize = size; }
    void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
    float GetScrollSpeed() const { return mScrollSpeed; }

  private:
    // Struct to encapsulate each bg image and its offset
    struct BGTexture {
        SDL_Texture *mTexture;
        Vector2 mOffset;
    };
    std::vector<BGTexture> mBGTextures;
    Vector2 mScreenSize;
    float mScrollSpeed;
};

#endif // __BGSPRITECOMPONENT_H__