#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// #include <stdio.h>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <list>

using namespace std;

#define CMD_INIT (100)
#define CMD_FINDPOSITION (200)
#define CMD_UPDATEMAP (300)

#include <algorithm>
#include <list>
#include <unordered_map>

using namespace std;

#define MAX_MAP_SIZE (500)
#define MAX_PIC_SIZE (8)

const int OFFSET = 16384;
#define MAX_N (500)
#define MAX_P (8)

struct RESULT
{
    int r, c;
};

int MAP[MAX_N][MAX_N];
const int MAX_HASH = 32769;
typedef pair<int, int> POINT;
POINT points[MAX_N * MAX_N * 2];
int point_counter;

int N;
int M;

// typedef unordered_map<int, list<POINT *>> HashMap;
typedef unordered_map<int, list<int>> HashMap;
HashMap HMAP;

struct Node
{
    int r;
    int c;
    Node *next;
    Node *prev;
};

Node nodes[2500000];
int node_cnt;
Node *getNode(int r, int c)
{
    Node *temp = &nodes[node_cnt++];
    temp->r = r;
    temp->c = c;
    temp->next = 0;
    temp->prev = 0;
    return temp;
}

struct MyHash
{
    Node *head;
    void init()
    {
        head = 0;
    }

    void addNode(Node *node)
    {
        if (head)
        {
            node->next = head;
            head->prev = node;
        }
        head = node;
    }
    Node *SearchNode(int r, int c)
    {
        Node *curr = head;
        while (curr)
        {
            if (curr->r == r && curr->c == c)
            {
                return curr;
            }
            curr = curr->next;
        }
        return 0;
    }

    void removeNode(Node *node)
    {
        if (head == node)
        {
            head = head->next;
            node->next = 0;
            node->prev = 0;
            node = 0;
            return;
        }
        if (node->prev)
        {
            node->prev->next = node->next;
        }
        if (node->next)
        {
            node->next->prev = node->prev;
        }
        node->next = 0;
        node->prev = 0;
        node = 0;
    }
};
MyHash myH[MAX_HASH];

struct PICS
{
    int PIC[MAX_P][MAX_P];
};
PICS pics[4];

bool isValid(int r, int c)
{
    if (r >= 0 && r < N && c >= 0 && c < N)
        return true;
    return false;
}
void init(int N, int M, int mMap[MAX_N][MAX_N])
{
    ::N = N;
    ::M = M;
    point_counter = 0;
    // HMAP.clear();
    for (register int i = 0; i < MAX_HASH; i++)
    {
        myH[i].init();
    }
    for (register int r = 0; r < N; r++)
    {
        for (register int c = 0; c < N; c++)
        {
            MAP[r][c] = mMap[r][c] + 16384;
            // points[point_counter] = {r, c};
            // HMAP[MAP[r][c]].push_back(point_counter);
            // point_counter++;
            Node *t = getNode(r, c);
            myH[MAP[r][c]].addNode(t);
        }
    }
}

bool FindMatch(int r, int c, int pix[MAX_P][MAX_P], int mValue, RESULT *ret)
{
    if (mValue < 0 || mValue >= MAX_HASH)
    {
        return false;
    }
    // HashMap::iterator hmapItr = HMAP.find(mValue);

    // if (hmapItr == HMAP.end())
    //     return false;
    Node *curr = myH[mValue].head;
    if (curr == 0)
        return false;

    // list<POINT *>::iterator listPointItr = hmapItr->second.begin();
    // list<int>::iterator listPointItr = hmapItr->second.begin();
    //while (listPointItr != hmapItr->second.end())
    while (curr)
    {
        // int r1 = points[(*listPointItr)].first;
        // int c1 = points[(*listPointItr)].second;

        int r1 = curr->r;
        int c1 = curr->c;
        int r2 = r1 + M - 1;
        int c2 = c1 + M - 1;
        if (false == isValid(r2, c2))
        {
            // listPointItr++;
            curr = curr->next;
        }
        else
        {
            int y, x, m, n;
            if (abs(pix[0][0] - MAP[r1][c1]) < 256 &&
                abs(pix[0][M - 1] - MAP[r1][c2]) < 256 &&
                abs(pix[M - 1][0] - MAP[r2][c1]) < 256 &&
                abs(pix[M - 1][M - 1] - MAP[r2][c2]) < 256)
            {
                bool status = true;

                int k = 0;
                for (y = r1; y < r2; y++)
                {
                    int l = 0;
                    for (x = c1; x < c2; x++)
                    {
                        if (abs(pix[k][l] - MAP[y][x]) >= 256)
                        {
                            return false;
                        }
                        l++;
                    }
                    k++;
                }
                if (status == true)
                {
                    ret->r = r1;
                    ret->c = c1;
                    return true;
                }
            }
            // listPointItr++;
            curr = curr->next;
        }
    }
    return false;
}

void rotateP(int mPic[MAX_PIC_SIZE][MAX_PIC_SIZE])
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            pics[0].PIC[i][j] = mPic[i][j] + OFFSET;
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
            pics[1].PIC[i][j] = pics[0].PIC[k][l];
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
            pics[2].PIC[i][j] = pics[1].PIC[k][l];
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
            pics[3].PIC[i][j] = pics[2].PIC[k][l];
            // cout << P[3].PIX[i][j] << " ";
            k--;
        }
        l++;
        // cout << endl;
    }
    // cout << endl;
}

void rotate(int mPic[MAX_PIC_SIZE][MAX_PIC_SIZE])
{
    for (register int r = 0; r < M; ++r)
    {
        for (register int c = 0; c < M; ++c)
        {
            pics[0].PIC[r][c] = mPic[r][c] + +16384;
            pics[1].PIC[r][c] = mPic[M - 1 - c][r] + +16384;
            pics[2].PIC[r][c] = mPic[M - 1 - r][M - 1 - c] + 16384;
            pics[3].PIC[r][c] = mPic[c][M - 1 - r] + 16384;
        }
    }
}

RESULT findPosition(int mPic[MAX_P][MAX_P])
{
    RESULT ret;
    ret.r = ret.c = -1;
    int cnt = 0;
    rotate(mPic);

    for (register int idx = 0; idx < 4; idx++)
    {
        for (register int i = 0; i < 256; i++)
        {
            bool status = false;
            ret.r = -1;
            ret.c = -1;
            int mValue = pics[idx].PIC[0][0] + i;
            status = FindMatch(0, 0, pics[idx].PIC, mValue, &ret);
            if (status == true)
            {
                return ret;
            }
            mValue = pics[idx].PIC[0][0] - i;
            status = FindMatch(0, 0, pics[idx].PIC, mValue, &ret);
            if (status == true)
            {
                return ret;
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
    bool operator()(POINT *p1)
    {
        if (r_ == p1->first && c_ == p1->second)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

void updateMap(int r, int c, int mNewValue)
{
    int val = MAP[r][c];
    Node *curr = myH[val].SearchNode(r, c);
    HashMap::iterator hmapItr = HMAP.find(val);
    // if (hmapItr != HMAP.end())
    if (curr)
    {
        myH[val].removeNode(curr);
        // list<POINT *>::iterator listPointItr = hmapItr->second.begin();
        // list<int>::iterator listPointItr = hmapItr->second.begin();
        // if (listPointItr != hmapItr->second.end())
        // {
        //     if (points[(*listPointItr)].first == r && points[(*listPointItr)].second == c)
        //     {
        //         hmapItr->second.erase(listPointItr);
        //     }
        // }
        // list<POINT *>::iterator lit = find_if(hmapItr->second.begin(), hmapItr->second.end(), Compare(r, c));
        // if (lit != hmapItr->second.end())
        // {
        //     hmapItr->second.erase(lit);
        // }
    }
    int mValue = mNewValue + 16384;
    MAP[r][c] = mValue;
    myH[mValue].addNode(getNode(r, c));
    // points[point_counter] = {r, c};
    // HMAP[mValue].push_back(point_counter);
    // point_counter++;
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
