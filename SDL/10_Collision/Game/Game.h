#ifndef __GAME_H__
#define __GAME_H__

#include "MyMath.h"
#include "SoundEvent.h"
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
    class AudioSystem *GetAudioSystem() { return mAudioSystem; }
    class PhysWorld *GetPhysWorld() { return mPhysWorld; }

    // Game-specific
    void AddPlane(class PlaneActor *plane);
    void RemovePlane(class PlaneActor *plane);
    std::vector<class PlaneActor *> &GetPlanes() { return mPlanes; }

  private:
    void ProcessInput();
    void HandleKeyPress(int key);
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    // All the actors in the game
    std::vector<class Actor *> mActors;
    // Any pending actors
    std::vector<class Actor *> mPendingActors;

    class Renderer *mRenderer;
    class AudioSystem *mAudioSystem;
    class PhysWorld *mPhysWorld;

    Uint32 mTicksCount;
    bool mIsRunning;
    // Track if we're updating actors right now
    bool mUpdatingActors;

    // Game-specific code
    std::vector<class PlaneActor *> mPlanes;
    class FPSActor *mFPSActor;
    class SpriteComponent *mCrosshair;
    SoundEvent mMusicEvent;
};

#endif // __GAME_H__