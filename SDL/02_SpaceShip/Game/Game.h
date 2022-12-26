#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

class Game {
  public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(class Actor *actor);
    void RemoveActor(class Actor *actor);

    void AddSprite(class SpriteComponent *sprite);
    void RemoveSprite(class SpriteComponent *sprite);

    SDL_Texture *GetTexture(const std::string &fileName);

  private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    // Map of textures loaded
    std::unordered_map<std::string, SDL_Texture *> mTextures;

    // All the actors in the game
    std::vector<class Actor *> mActors;
    // Any pending actors
    std::vector<class Actor *> mPendingActors;

    // All the sprite components drawn
    std::vector<class SpriteComponent *> mSprites;

    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;
    // Track if we're updating actors right now
    bool mUpdatingActors;

    // Game-specific
    class Ship *mShip; // Player's ship
};

#endif // __GAME_H__