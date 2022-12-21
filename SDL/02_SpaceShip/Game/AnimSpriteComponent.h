#ifndef __ANIMSPRITECOMPONENT_H__
#define __ANIMSPRITECOMPONENT_H__

#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent {
  public:
    AnimSpriteComponent(class Actor *owner, int drawOrder = 100);
    // Update animation every frame (overridden from component)
    void Update(float deltaTime) override;
    // Set the textures used for animation
    void SetAnimTextures(const std::vector<SDL_Texture *> &textures);
    // Set/get the animation FPS
    float GetAnimFPS() const { return mAnimFPS; }
    void SetAnimFPS(float fps) { mAnimFPS = fps; }

  private:
    // All textures in the animation
    std::vector<SDL_Texture *> mAnimTextures;
    // Current frame displayed
    float mCurrFrame;
    // Animation frame rate
    float mAnimFPS;
};

#endif // __ANIMSPRITECOMPONENT_H__