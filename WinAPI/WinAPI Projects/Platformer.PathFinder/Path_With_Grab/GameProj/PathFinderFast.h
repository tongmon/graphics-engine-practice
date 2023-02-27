#pragma once

#include <Windows.h>
#include <queue>
#include <stack>

class Map;

enum HeuristicFormula
{
    Manhattan = 1,
    MaxDXDY = 2,
    DiagonalShortCut = 3,
    Euclidean = 4,
    EuclideanNoSQR = 5,
    Custom1 = 6
};

class PathFinderFast
{
    // 길찾기 가중치가 되는 노드
    typedef struct _pathf
    {
        int F;
        int G;
        short PX;
        short PY;
        short PZ;
        short Status;
        short JumpLength;
    }PathFinderNodeFast;

    // 우선 순위 큐에 위치 자료구조
    typedef struct _loc
    {
        int xy;
        int z;
    }Location;

    class ComparePFNodeMatrix
    {
        std::vector<std::vector<PathFinderNodeFast>>* mMatrix;

    public:
        ComparePFNodeMatrix(std::vector<std::vector<PathFinderNodeFast>>* Matrix)
        {
            mMatrix = Matrix;
        }
        bool operator()(Location a, Location b)
        {
            return mMatrix->at(a.xy).at(a.z).F > mMatrix->at(b.xy).at(b.z).F;
        }
    };

    std::vector<std::vector<PathFinderNodeFast>> nodes;
    std::stack<int> touchedLocations; // 알고리즘이 통과할 목록을 지우는 경우에 사용

    std::priority_queue<Location, std::vector<Location>, ComparePFNodeMatrix>* mOpen;
    std::vector<POINT>                   mClose;
    std::vector<std::vector<short>>*     mGrid; // 맵 격자
    bool                            mStop = false;
    bool                            mStopped = true;
    HeuristicFormula                mFormula = HeuristicFormula::Manhattan;
    bool                            mDiagonals = true;
    int                             mHEstimate = 2;
    // bool                            mPunishChangeDirection = false;
    bool                            mTieBreaker = false;
    bool                            mHeavyDiagonals = false;
    int                             mSearchLimit = 2000;
    // double                          mCompletedTime = 0;
    // bool                            mDebugProgress = false;
    // bool                            mDebugFoundPath = false;
    int                             mOpenNodeValue = 1;
    int                             mCloseNodeValue = 2;

    int                             mH = 0;
    Location                        mLocation;
    int                             mNewLocation = 0;
    unsigned short                  mLocationX = 0;
    unsigned short                  mLocationY = 0;
    unsigned short                  mNewLocationX = 0;
    unsigned short                  mNewLocationY = 0;
    int                             mCloseNodeCounter = 0;
    unsigned short                  mGridX = 0;
    unsigned short                  mGridY = 0;
    bool                            mFound = false;
    short                           mDirection[8][2] = { {0,-1} , {1,0}, {0,1}, {-1,0}, {1,-1}, {1,1}, {-1,1}, {-1,-1} };
    int                             mEndLocation = 0;
    int                             mNewG = 0;

    short                           cLedgeGrabJumpValue = -9;

    Map*                            mMap = NULL;

public:
    PathFinderFast();
    ~PathFinderFast();
    void InitPathFinder(Map* Map);
    void FindPathStop();
    short GetGrid(int x, int y);
    std::vector<POINT>* FindPath(POINT start, POINT end, int characterWidth, int characterHeight, short maxCharacterJumpHeight, bool useLedges);
};