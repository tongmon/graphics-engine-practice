#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

class Texture {
  public:
    Texture();
    ~Texture();

    bool Load(const std::string &fileName);
    void Unload();

    void SetActive();

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }

  private:
    // OpenGL ID of this texture
    unsigned int mTextureID;
    // Width/height of the texture
    int mWidth;
    int mHeight;
};

#endif // __TEXTURE_H__