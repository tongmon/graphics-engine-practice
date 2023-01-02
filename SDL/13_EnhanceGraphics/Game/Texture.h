#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

class Texture {
  public:
    Texture();
    ~Texture();

    bool Load(const std::string &fileName);
    void Unload();
    void CreateFromSurface(struct SDL_Surface *surface);
    void CreateForRendering(int width, int height, unsigned int format);

    void SetActive(int index = 0);

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
    unsigned int GetTextureID() const { return mTextureID; }

  private:
    unsigned int mTextureID;
    int mWidth;
    int mHeight;
};

#endif // __TEXTURE_H__