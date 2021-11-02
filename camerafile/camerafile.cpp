#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// #include <stdio.h>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <list>

using namespace std;

#define MAX_MAP_SIZE (500)
#define MAX_PIC_SIZE (8)

#define CMD_INIT (100)
#define CMD_FINDPOSITION (200)
#define CMD_UPDATEMAP (300)

struct RESULT
{
    int r, c;
};

const int MAX_HASH = 2 ^ 15 + 1;
const int OFFSET = 16384;
int N;
int M;

int MAP[MAX_MAP_SIZE][MAX_MAP_SIZE];

typedef pair<int, int> POINT;
POINT points[MAX_MAP_SIZE * MAX_MAP_SIZE];

int pointsCnt;
unordered_map<int, list<POINT *>> pointsMap;

void init(int n, int m, int mMap[MAX_MAP_SIZE][MAX_MAP_SIZE])
{
    ::N = n;
    ::M = m;
    pointsCnt = 0;
    pointsMap.clear();

    for (int r = 0; r < N; r++)
    {
        for (int c = 0; c < N; c++)
        {
            if (mMap[r][c] + OFFSET == 29404)
            {
                cout << r << "," << c << "  "
                     << " pVal=" << mMap[r][c] + OFFSET << endl;
            }
            MAP[r][c] = mMap[r][c] + OFFSET;
            points[pointsCnt].first = r;
            points[pointsCnt].second = c;
            pointsMap[MAP[r][c]].push_back(&points[pointsCnt]);
            pointsCnt++;
        }
    }
}

struct PICS
{
    int PIX[MAX_PIC_SIZE][MAX_PIC_SIZE];
};
PICS P[4];
void rotateP(int mPic[MAX_PIC_SIZE][MAX_PIC_SIZE])
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            P[0].PIX[i][j] = mPic[i][j] + OFFSET;
            // cout << P[0].PIX[i][j] << " ";
        }
        // cout << endl;
    }
    // cout << endl;

    int k = M - 1;
    int l = 0;
    for (int i = 0; i < M; i++)
    {
        k = M - 1;
        for (int j = 0; j < M; j++)
        {
            P[1].PIX[i][j] = P[0].PIX[k][l];
            // cout << P[1].PIX[i][j] << " ";
            k--;
        }
        l++;
        // cout << endl;
    }
    // cout << endl;

    k = M - 1;
    l = 0;
    for (int i = 0; i < M; i++)
    {
        k = M - 1;
        for (int j = 0; j < M; j++)
        {
            P[2].PIX[i][j] = P[1].PIX[k][l];
            // cout << P[2].PIX[i][j] << " ";
            k--;
        }
        l++;
        // cout << endl;
    }
    // cout << endl;

    k = M - 1;
    l = 0;
    for (int i = 0; i < M; i++)
    {
        k = M - 1;
        for (int j = 0; j < M; j++)
        {
            P[3].PIX[i][j] = P[2].PIX[k][l];
            // cout << P[3].PIX[i][j] << " ";
            k--;
        }
        l++;
        // cout << endl;
    }
    // cout << endl;
}

bool valid(int r, int c)
{
    if (r >= 0 && r < MAX_PIC_SIZE && c >= 0 && c < MAX_PIC_SIZE)
        return true;
    return false;
}

bool checkPic(int pix[MAX_PIC_SIZE][MAX_PIC_SIZE], int r, int c)
{
    bool match = false;
    // N is map; M is pix; (r,c) MAP co-ordinates;
    int r2 = r + M - 1;
    int c2 = c + M - 1;
    // invalidate r2, c2;
    if (valid(r2, c2) == false)
    {
        return false;
    }
    // all four cornors are a match
    if (abs(pix[0][0] - MAP[r][c]) < 256 &&
        abs(pix[0][M - 1] - MAP[r][c2]) < 256 &&
        abs(pix[M - 1][0] - MAP[r2][c]) < 256 &&
        abs(pix[M - 1][M - 1] - MAP[r2][c2]) < 256)
    {
        match = true;
        int k = 0;
        int l = 0;
        for (int x = r; x < r2; x++)
        {
            l = 0;
            for (int y = c; y < c2; y++)
            {
                if (abs(pix[k][l] - MAP[x][y]) >= 256)
                {
                    match = false;
                    return false;
                }
                l++;
            }
            k++;
        }
        if (match == true)
        {
            return true;
        }
    }
    return false;
}

bool findWithPVal(int pVal, int pix[MAX_PIC_SIZE][MAX_PIC_SIZE], RESULT *ret)
{
    int r = 0;
    int c = 0;
    unordered_map<int, list<POINT *>>::iterator it = pointsMap.find(pVal);
    if (it != pointsMap.end())
    {
        list<POINT *> plist = it->second;
        list<POINT *>::iterator itr = plist.begin();
        POINT *p = 0;
        while (itr != plist.end())
        {
            p = *itr;
            r = p->first;
            c = p->second;
            if (checkPic(pix, r, c) == true)
            {
                ret->r = r;
                ret->c = c;
                return true;
            }
            itr++;
        }
    }
    return false;
}

RESULT findPosition(int mPic[MAX_PIC_SIZE][MAX_PIC_SIZE])
{
    RESULT ret;

    ret.r = ret.c = -1;
    // 1. Rotate Pic 0, 90, 180, 270
    rotateP(mPic);

    // for each intensity
    int r = 0;
    int c = 0;
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 256; i++)
        {
            int pVal = P[j].PIX[0][0] + i;
            if (pVal == 590) //29404)
            {
                cout << "start from here" << endl;
            }

            unordered_map<int, list<POINT *>>::iterator it = pointsMap.find(pVal);
            if (it != pointsMap.end())
            {
                list<POINT *> plist = it->second;
                list<POINT *>::iterator itr = plist.begin();
                POINT *p = 0;
                while (itr != plist.end())
                {
                    p = *itr;
                    r = p->first;
                    c = p->second;
                    if (checkPic(pix, r, c) == true)
                    {
                        ret->r = r;
                        ret->c = c;
                        return true;
                    }
                    itr++;
                }
            }
        }
    }
    return ret;
}

struct Compare
{
    int r_;
    int c_;
    Compare(int r, int c) : r_(r), c_(c){};
    bool operator()(POINT *p)
    {
        if (p->first == r_ && p->second == c_)
        {
            return true;
        }
        return false;
    }
};

void updateMap(int r, int c, int mNewValue)
{
    int oldValue = MAP[r][c];
    unordered_map<int, list<POINT *>>::iterator it = pointsMap.find(oldValue);
    if (it != pointsMap.end())
    {
        list<POINT *>::iterator lit = find_if(it->second.begin(), it->second.end(), Compare(r, c));
        if (lit != it->second.end())
        {
            it->second.erase(lit);
        }
        points[pointsCnt] = {r, c};
        pointsMap[mNewValue + OFFSET].push_back(&points[pointsCnt]);
        pointsCnt++;
    }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int mSeed;

static int pseudo_rand(void)
{
    mSeed = mSeed * 214013 + 2531011;
    return (mSeed >> 16) & 0x7FFF;
}

static int oMap[MAX_MAP_SIZE][MAX_MAP_SIZE];

static void makeMap(int N, int seed, int mMap[MAX_MAP_SIZE][MAX_MAP_SIZE])
{
    mSeed = seed;

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            mMap[i][j] = (pseudo_rand() % 32768) - 16384;
}

static RESULT makePic(int seed, int N, int M, int mPic[MAX_PIC_SIZE][MAX_PIC_SIZE])
{
    static int org[MAX_PIC_SIZE][MAX_PIC_SIZE];

    mSeed = seed;

    int r = pseudo_rand() % (N - M + 1);
    int c = pseudo_rand() % (N - M + 1);
    int dir = pseudo_rand() % 4;

    for (int dr = 0; dr < M; ++dr)
        for (int dc = 0; dc < M; ++dc)
        {
            org[dr][dc] = oMap[r + dr][c + dc] + (pseudo_rand() % 256) - 128;

            if (org[dr][dc] < -16384)
                org[dr][dc] = -16384;
            if (org[dr][dc] > 16383)
                org[dr][dc] = 16383;
        }

    if (dir == 0)
    {
        for (int dr = 0; dr < M; ++dr)
            for (int dc = 0; dc < M; ++dc)
                mPic[dr][dc] = org[dr][dc];
    }
    else if (dir == 1)
    {
        for (int dr = 0; dr < M; ++dr)
            for (int dc = 0; dc < M; ++dc)
                mPic[dr][dc] = org[M - 1 - dc][dr];
    }
    else if (dir == 2)
    {
        for (int dr = 0; dr < M; ++dr)
            for (int dc = 0; dc < M; ++dc)
                mPic[dr][dc] = org[M - 1 - dr][M - 1 - dc];
    }
    else
    {
        for (int dr = 0; dr < M; ++dr)
            for (int dc = 0; dc < M; ++dc)
                mPic[dr][dc] = org[dc][M - 1 - dr];
    }

    RESULT ret;

    ret.r = r;
    ret.c = c;

    return ret;
}

static bool run()
{
    static int mMap[MAX_MAP_SIZE][MAX_MAP_SIZE];
    static int mPic[MAX_PIC_SIZE][MAX_PIC_SIZE];

    int N, M, Q;
    int cmd, seed;
    int r, c;

    RESULT ans, ret;

    scanf("%d %d %d %d", &Q, &N, &M, &seed);

    bool okay = false;

    makeMap(N, seed, mMap);

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            oMap[i][j] = mMap[i][j];

    for (int q = 0; q <= Q; ++q)
    {
        scanf("%d", &cmd);
        switch (cmd)
        {
        case CMD_INIT:
            init(N, M, mMap);
            okay = true;
            break;
        case CMD_FINDPOSITION:
            scanf("%d", &seed);
            ans = makePic(seed, N, M, mPic);
            if (okay)
            {
                ret = findPosition(mPic);
                if (ret.r != ans.r || ret.c != ans.c)
                    okay = false;
            }
            break;
        case CMD_UPDATEMAP:
            scanf("%d", &mSeed);
            r = pseudo_rand() % N;
            c = pseudo_rand() % N;
            oMap[r][c] = (pseudo_rand() % 32768) - 16384;
            if (okay)
                updateMap(r, c, oMap[r][c]);
            break;
        default:
            okay = false;
            break;
        }
    }

    return okay;
}

int main()
{
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; ++tc)
    {
        int score = run() ? MARK : 0;

        printf("#%d %d\n", tc, score);
    }
    return 0;
}
