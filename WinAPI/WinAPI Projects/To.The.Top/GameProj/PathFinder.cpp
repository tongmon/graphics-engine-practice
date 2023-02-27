#include "PathFinder.h"
#include "GameMap.h"

PathFinder::PathFinder()
{
    mOpen = NULL;
}

PathFinder::~PathFinder()
{
    if (mOpen != NULL)
        delete mOpen;
}

void PathFinder::InitPathFinder(GameMap* Map)
{
    if (Map == NULL) {
        return;
    }

    mPathMap = Map;

    mGrid = &mPathMap->mGrid;
    mGridX = (unsigned short)mGrid->at(0).size();
    mGridY = (unsigned short)mGrid->size();

    nodes.resize(mGridX * mGridY);
    mClose.reserve(mGridX * mGridY);

    mOpen = new std::priority_queue<Location, std::vector<Location>, ComparePFNodeMatrix>(ComparePFNodeMatrix(&nodes));

    mFormula = HeuristicFormula::Euclidean;
    mDiagonals = false;
    mHeavyDiagonals = false;
    mHEstimate = 6;
    mSearchLimit = 100000;
}

void PathFinder::FindPathStop()
{
    mStop = true;
}

short PathFinder::GetGrid(int x, int y)
{
    if (x < 1 || x >= mPathMap->mWidth - 1 || y < 1 || y >= mPathMap->mHeight - 1)
        return 0;
    return mGrid->at(y).at(x);
}

std::vector<POINT>* PathFinder::FindPath(POINT start, POINT end, int characterWidth, int characterHeight, short maxCharacterJumpHeight, bool useLedges)
{
    while (!mOpen->empty())
        mOpen->pop();

    // ������ �ǵ帰 ��ġ ��� ����
    while (!touchedLocations.empty())
    {
        int Top = touchedLocations.top();
        touchedLocations.pop();
        nodes[Top].clear();
    }

    bool inSolidTile = false;

    for (int i = 0; i < 2; ++i)
    {
        inSolidTile = false;
        for (int w = 0; w < characterWidth; ++w)
        {
            if (GetGrid(end.x + w, end.y) == 0
                || GetGrid(end.x + w, end.y + characterHeight - 1) == 0)
            {
                inSolidTile = true;
                break;
            }
        }
        if (inSolidTile == false)
        {
            for (int h = 1; h < characterHeight - 1; ++h)
            {
                if (GetGrid(end.x, end.y + h) == 0
                    || GetGrid(end.x + characterWidth - 1, end.y + h) == 0)
                {
                    inSolidTile = true;
                    break;
                }
            }
        }

        if (inSolidTile)
            end.x -= characterWidth - 1;
        else
            break;
    }

    if (inSolidTile == true)
        return NULL;

    mFound = false;
    mStop = false;
    mStopped = false;
    mCloseNodeCounter = 0;
    mOpenNodeValue += 2;
    mCloseNodeValue += 2;

    if (mGridX < mGridY) {
        mLocation.xy = start.y * mGridX + start.x; // ���� x, y�� ������ ��ȯ
        mEndLocation = end.y * mGridX + end.x; // �� x, y�� ������ ��ȯ
    }
    else {
        mLocation.xy = start.x * mGridY + start.y; // ���� x, y�� ������ ��ȯ
        mEndLocation = end.x * mGridY + end.y; // �� x, y�� ������ ��ȯ
    }

    mLocation.z = 0;

    PathFinderNodeFast firstNode;
    firstNode.G = 0;
    firstNode.F = mHEstimate;
    firstNode.PX = (unsigned short)start.x;
    firstNode.PY = (unsigned short)start.y;
    firstNode.PZ = 0; // ���� ����̱⿡ PZ�� 0���� ����
    firstNode.Status = mOpenNodeValue;

    bool startsOnGround = false;

    for (int x = start.x; x < start.x + characterWidth; ++x)
    {
        if (mPathMap->IsGround(x, start.y - 1))
        {
            startsOnGround = true;
            break;
        }
    }

    if (startsOnGround)
        firstNode.JumpLength = 0;
    else
        firstNode.JumpLength = (short)(maxCharacterJumpHeight * 2);

    nodes[mLocation.xy].push_back(firstNode); // ù ��� �߰�
    touchedLocations.push(mLocation.xy); // ���� ���� �ǵ帰 ����̹Ƿ� ���ÿ� �߰�

    mOpen->push(mLocation); // �켱���� ť�� ���� ��ġ ����

    // A* �˰��� ���� �켱���� ť�� �̿��� �ִ� �Ÿ� ȹ��
    while (!mOpen->empty() && !mStop)
    {
        mLocation = mOpen->top();
        mOpen->pop();

        // ���� ��忡 �� ���¸� �̹� ó���Ǿ��⿡ ��ŵ
        if (nodes[mLocation.xy][mLocation.z].Status == mCloseNodeValue)
            continue;

        // xy�� ������ mLocation.xy���� x, y�� �и��ؼ� ȹ���Ѵ�.
        if (mGridX < mGridY) {
            mLocationX = unsigned short(mLocation.xy % mGridX);
            mLocationY = unsigned short(mLocation.xy / mGridX);
        }
        else {
            mLocationX = unsigned short(mLocation.xy / mGridY);
            mLocationY = unsigned short(mLocation.xy % mGridY);
        }

        // �������� ���������� ã�����Ƿ� ��ã�� ����
        if (mLocation.xy == mEndLocation)
        {
            nodes[mLocation.xy][mLocation.z].Status = mCloseNodeValue;
            mFound = true;
            break;
        }

        // mSearchLimit�� �Ѿ���� Ž���� ����Ǹ� Ž���� �׸��д�.
        if (mCloseNodeCounter > mSearchLimit)
        {
            mStopped = true;
            return NULL;
        }

        // ������ ���
        // ��� �ÿ� �����ڰ� � �Ӽ��� �ִ��� �˾ƾ� �Ѵ�.
        // 1. ĳ���Ͱ� ���� ���� �ʴ� ���� �ִ� ��� --> �ļ� �۾��� ������.
        // 2. ĳ���Ͱ� ��ġ�� ������ �ٴڿ� �ִ� --> �������� 0���� ����
        // 3. ĳ���Ͱ� ��ġ�� �°� õ�� �ٷ� �Ʒ��� �ִ� --> õ�� �ٷ� �Ʒ��� �����Ƿ� ������ �����ص� ���ϰ� �����.
        // 4. ĳ���ʹ� ���� �°� �ְ� �ٴ��̳� õ�忡 ���� �ʴ� --> ���� ����
        for (int i = 0; i < (mDiagonals ? 8 : 4); i++)
        {
            mNewLocationX = unsigned short(mLocationX + mDirection[i][0]); // Ž���� X
            mNewLocationY = unsigned short(mLocationY + mDirection[i][1]); // Ž���� Y

            if (mGridX < mGridY) {
                mNewLocation = mNewLocationY * mGridX + mNewLocationX; // X, Y ������ ��ȯ
            }
            else {
                mNewLocation = mNewLocationX * mGridY + mNewLocationY;
            }

            int jumpLength = nodes[mLocation.xy][mLocation.z].JumpLength;
            short newJumpLength = jumpLength;

            bool onGround = false;
            bool atCeiling = false;

            for (int w = 0; w < characterWidth; ++w)
            {
                if (GetGrid(mNewLocationX + w, mNewLocationY) == 0
                    || GetGrid(mNewLocationX + w, mNewLocationY + characterHeight - 1) == 0)
                    goto CHILDREN_LOOP_END;

                if (mPathMap->IsGround(mNewLocationX + w, mNewLocationY - 1))
                    onGround = true;
                else if (GetGrid(mNewLocationX + w, mNewLocationY + characterHeight) == 0)
                    atCeiling = true;
            }
            for (int h = 1; h < characterHeight - 1; ++h)
            {
                if (GetGrid(mNewLocationX, mNewLocationY + h) == 0
                    || GetGrid(mNewLocationX + characterWidth - 1, mNewLocationY + h) == 0)
                    goto CHILDREN_LOOP_END;
            }

            // ĳ������ �Ӹ��� õ��� ��� �ִ� ���
            // 1. ĳ���ʹ� �������� ������ �� �ִ�.
            // 2. ĳ���ʹ� �¿�� ������ �� �ִ�.
            // �� �ΰ��� ��츸 �����ϴ�.
            if (onGround)
                newJumpLength = 0;
            else if (atCeiling)
            {
                if (mNewLocationX != mLocationX)
                    newJumpLength = (short)max(maxCharacterJumpHeight * 2 + 1, jumpLength + 1);
                else
                    newJumpLength = (short)max(maxCharacterJumpHeight * 2, jumpLength + 2);
            }
            else if (useLedges && jumpLength > 0
                && jumpLength <= maxCharacterJumpHeight * 2 + 6
                && ((GetGrid(mNewLocationX + characterWidth, mNewLocationY + characterHeight - 1) == 0 && GetGrid(mNewLocationX + characterWidth, mNewLocationY + characterHeight) != 0)
                    || (GetGrid(mNewLocationX - 1, mNewLocationY + characterHeight - 1) == 0 && GetGrid(mNewLocationX - 1, mNewLocationY + characterHeight) != 0)))
            {
                newJumpLength = cLedgeGrabJumpValue;
            }
            else if (mNewLocationY > mLocationY)
            {
                if (jumpLength == cLedgeGrabJumpValue)
                    newJumpLength = 3;
                else if (jumpLength < 2 && maxCharacterJumpHeight > 2) //first jump is always two block up instead of one up and optionally one to either right or left
                    newJumpLength = 3;
                else if (jumpLength % 2 == 0)
                    newJumpLength = (short)(jumpLength + 2);
                else
                    newJumpLength = (short)(jumpLength + 1);
            }
            else if (mNewLocationY < mLocationY)
            {
                if (jumpLength == cLedgeGrabJumpValue)
                    newJumpLength = (short)(maxCharacterJumpHeight * 2 + 4);
                else if (jumpLength % 2 == 0)
                    newJumpLength = (short)max(maxCharacterJumpHeight * 2, jumpLength + 2);
                else
                    newJumpLength = (short)max(maxCharacterJumpHeight * 2, jumpLength + 1);
            }
            else if (!onGround && mNewLocationX != mLocationX)
            {
                if (jumpLength == cLedgeGrabJumpValue)
                    newJumpLength = (short)(maxCharacterJumpHeight * 2 + 3);
                else
                    newJumpLength = (short)max(jumpLength + 1, 1);
            }

            if (jumpLength == cLedgeGrabJumpValue && mLocationX != mNewLocationX && newJumpLength < maxCharacterJumpHeight * 2)
                continue;

            if (jumpLength >= 0 && jumpLength % 2 != 0 && mLocationX != mNewLocationX)
                continue;

            if ((newJumpLength == 0 && mNewLocationX != mLocationX && jumpLength + 1 >= maxCharacterJumpHeight * 2 + 6 && (jumpLength + 1 - (maxCharacterJumpHeight * 2 + 6)) % 8 <= 1)
                || (newJumpLength >= maxCharacterJumpHeight * 2 + 6 && mNewLocationX != mLocationX && (newJumpLength - (maxCharacterJumpHeight * 2 + 6)) % 8 != 7))
                continue;

            //if we're falling and succeor's height is bigger than ours, skip that successor
            if (jumpLength >= maxCharacterJumpHeight * 2 && mNewLocationY > mLocationY)
                continue;

            mNewG = nodes[mLocation.xy][mLocation.z].G + GetGrid(mNewLocationX, mNewLocationY) + newJumpLength / 4;

            if (!nodes[mNewLocation].empty())
            {
                int lowestJump = 32000;
                int lowestG = 32000;
                bool couldMoveSideways = false;
                for (size_t j = 0; j < nodes[mNewLocation].size(); ++j)
                {
                    if (nodes[mNewLocation][j].JumpLength < lowestJump)
                        lowestJump = nodes[mNewLocation][j].JumpLength;

                    if (nodes[mNewLocation][j].G < lowestG)
                        lowestG = nodes[mNewLocation][j].G;

                    if (nodes[mNewLocation][j].JumpLength % 2 == 0 && nodes[mNewLocation][j].JumpLength < maxCharacterJumpHeight * 2 + 6)
                        couldMoveSideways = true;
                }

                // The current node has smaller cost than the previous? then skip this node
                if (lowestG <= mNewG && lowestJump <= newJumpLength && (newJumpLength % 2 != 0 || newJumpLength >= maxCharacterJumpHeight * 2 + 6 || couldMoveSideways))
                    continue;
            }

            // mFormula�� ���� ���� �ٸ� H ���� ����ϰ� �ȴ�.
            switch (mFormula)
            {
            default:
            case HeuristicFormula::Manhattan:
            {
                mH = mHEstimate * (abs(mNewLocationX - end.x) + abs(mNewLocationY - end.y));
                break;
            }
            case HeuristicFormula::MaxDXDY:
            {
                mH = mHEstimate * (max(abs(mNewLocationX - end.x), abs(mNewLocationY - end.y)));
                break;
            }
            case HeuristicFormula::DiagonalShortCut:
            {
                int h_diagonal = min(abs(mNewLocationX - end.x), abs(mNewLocationY - end.y));
                int h_straight = (abs(mNewLocationX - end.x) + abs(mNewLocationY - end.y));
                mH = (mHEstimate * 2) * h_diagonal + mHEstimate * (h_straight - 2 * h_diagonal);
                break;
            }
            case HeuristicFormula::Euclidean:
            {
                mH = (int)(mHEstimate * sqrt(pow((mNewLocationY - end.x), 2) + pow((mNewLocationY - end.y), 2)));
                break;
            }
            case HeuristicFormula::EuclideanNoSQR:
            {
                mH = (int)(mHEstimate * (pow((mNewLocationX - end.x), 2) + pow((mNewLocationY - end.y), 2)));
                break;
            }
            case HeuristicFormula::Custom1:
            {
                POINT dxy = { abs(end.x - mNewLocationX), abs(end.y - mNewLocationY) };
                int Orthogonal = abs(dxy.x - dxy.y);
                int Diagonal = abs(((dxy.x + dxy.y) - Orthogonal) / 2);
                mH = mHEstimate * (Diagonal + Orthogonal + dxy.x + dxy.y);
                break;
            }
            }

            PathFinderNodeFast newNode;
            newNode.JumpLength = newJumpLength;
            newNode.PX = mLocationX;
            newNode.PY = mLocationY;
            newNode.PZ = mLocation.z;
            newNode.G = mNewG;
            newNode.F = mNewG + mH;
            newNode.Status = mOpenNodeValue;

            if (nodes[mNewLocation].size() == 0)
                touchedLocations.push(mNewLocation);

            nodes[mNewLocation].push_back(newNode);
            mOpen->push({ mNewLocation, (int)nodes[mNewLocation].size() - 1 });

        CHILDREN_LOOP_END:
            continue;
        }

        nodes[mLocation.xy][mLocation.z].Status = mCloseNodeValue;
        mCloseNodeCounter++;
    }

    // ������ ��� ��尡 �̵��� ������ �ʴ´�.
    // ���� �� �ʿ��� ��常 ����� �������� ������.
    if (mFound)
    {
        mClose.clear();
        int posX = end.x;
        int posY = end.y;

        PathFinderNodeFast fPrevNodeTmp;
        PathFinderNodeFast fNodeTmp = nodes[mEndLocation][0];

        POINT fNode = end;
        POINT fPrevNode = end;

        int loc;
        if (mGridX < mGridY) {
            loc = fNodeTmp.PY * mGridX + fNodeTmp.PX;
        }
        else {
            loc = fNodeTmp.PX * mGridY + fNodeTmp.PY;
        }

        while (fNode.x != fNodeTmp.PX || fNode.y != fNodeTmp.PY)
        {
            PathFinderNodeFast fNextNodeTmp = nodes[loc][fNodeTmp.PZ];

            if ((mClose.empty())
                || (mPathMap->IsOneWayPlatform(fNode.x, fNode.y - 1))
                || (GetGrid(fNode.x, fNode.y - 1) == 0 && mPathMap->IsOneWayPlatform(fPrevNode.x, fPrevNode.y - 1))
                || (fNodeTmp.JumpLength == 3)
                || (fNextNodeTmp.JumpLength != 0 && fNodeTmp.JumpLength == 0)                                                                                                       //mark jumps starts
                || (fNodeTmp.JumpLength == 0 && fPrevNodeTmp.JumpLength != 0)                                                                                                       //mark landings
                || (fNode.y > mClose[(int)mClose.size() - 1].y && fNode.y > fNodeTmp.PY)
                || (fNodeTmp.JumpLength == cLedgeGrabJumpValue)
                || (fNode.y < mClose[(int)mClose.size() - 1].y && fNode.y < fNodeTmp.PY)
                || ((mPathMap->IsGround(fNode.x - 1, fNode.y) || mPathMap->IsGround(fNode.x + 1, fNode.y))
                    && fNode.y != mClose[(int)mClose.size() - 1].y && fNode.x != mClose[(int)mClose.size() - 1].x))
                mClose.push_back(fNode);

            fPrevNode = fNode;
            posX = fNodeTmp.PX;
            posY = fNodeTmp.PY;
            fPrevNodeTmp = fNodeTmp;
            fNodeTmp = fNextNodeTmp;
            if (mGridX < mGridY) {
                loc = fNodeTmp.PY * mGridX + fNodeTmp.PX;
            }
            else {
                loc = fNodeTmp.PX * mGridY + fNodeTmp.PY;
            }
            fNode = { posX, posY };
        }

        mClose.push_back(fNode);

        mStopped = true;

        return &mClose;
    }
    mStopped = true;
    return NULL;
}