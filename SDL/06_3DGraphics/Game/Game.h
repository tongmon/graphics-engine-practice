#ifndef __GAME_H__
#define __GAME_H__

#include "MyMath.h"
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

    class Renderer *GetRenderer() { return mRenderer; }

  private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    // All the actors in the game
    std::vector<class Actor *> mActors;
    // Any pending actors
    std::vector<class Actor *> mPendingActors;

    class Renderer *mRenderer;

    Uint32 mTicksCount;
    bool mIsRunning;
    // Track if we're updating actors right now
    bool mUpdatingActors;

    // Game-specific code
    class CameraActor *mCameraActor;
};

#endif // __GAME_H__