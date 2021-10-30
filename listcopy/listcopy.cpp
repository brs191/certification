#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <iostream>
#include <unordered_map>

using namespace std;

#define CMD_INIT 100
#define CMD_MAKE_LIST 200
#define CMD_COPY_LIST 300
#define CMD_UNDATE_ELEMENT 400
#define CMD_ELEMENT 500

const int MAX_LIST_CNT = 10;
const int MAX_LIST_LEN = 200000;
const int MAX_NAMES = 6000;
const int MAX_CHANGES = 110000;

int initNumber;
int initValue[MAX_LIST_CNT + 1][MAX_LIST_LEN];

int addressNumber;
unordered_map<string, int> address;

int changeNumber;
pair<int, int> changeLog[MAX_CHANGES + 1];
int lastChange[MAX_NAMES + 1];
int prevChange[MAX_CHANGES + 1];

void init()
{
    initNumber = 0;
    addressNumber = 0;
    address.clear();
    changeNumber = 0;
    for (int i = 0; i < MAX_CHANGES; i++)
    {
        changeLog->first = -1;
        changeLog->second = -1;
    }
}

void makeList(char mName[], int mLength, int mListValue[])
{
    memcpy(initValue[initNumber], mListValue, mLength * sizeof(int));

    address[string(mName)] = addressNumber;

    changeLog[changeNumber] = {-1, initNumber};
    prevChange[address[string(mName)]] = -1;
    lastChange[address[string(mName)]] = changeNumber;

    changeNumber++;
    addressNumber++;
    initNumber++;
}

void copyList(char mDest[], char mSrc[], bool mCopy)
{
    if (mCopy)
    {
        address[string(mDest)] = addressNumber;

        changeLog[changeNumber] = {-1, -1};
        prevChange[changeNumber] = lastChange[address[string(mSrc)]];
        lastChange[address[string(mDest)]] = changeNumber;

        changeNumber++;
        addressNumber++;
    }
    else
    {
        address[string(mDest)] = address[string(mSrc)];
    }
}

void updateElement(char mName[], int mIndex, int mValue)
{
    changeLog[changeNumber] = {mIndex, mValue};

    prevChange[changeNumber] = lastChange[address[string(mName)]]; // why this??
    lastChange[address[string(mName)]] = changeNumber;
    changeNumber++;
}

int element(char mName[], int mIndex)
{

    int lc = lastChange[address[string(mName)]];
    while (true)
    {
        if (prevChange[lc] == -1) // we reached to original
        {
            return initValue[changeLog[lc].second][mIndex];
        }
        else if (changeLog[lc].first == mIndex)
        {
            return changeLog[lc].second;
        }
        lc = prevChange[lc];
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static unsigned int mSeed;
static unsigned int pseudo_rand(void)
{
    mSeed = mSeed * 214013 + 2531011;
    return (mSeed >> 16) & 0x7FFF;
}

static char mName[21];
static char mDest[21];
static char mSrc[21];
static int mListValue[200000];

static void generateName(char *name, int seed)
{
    mSeed = seed;
    int name_len = pseudo_rand() % 20 + 1;
    for (int i = 0; i < name_len; ++i)
    {
        name[i] = pseudo_rand() % 26 + 'a';
    }
    name[name_len] = '\0';
}

static int generateList(int *listValue, int seed)
{
    mSeed = seed;
    int length = pseudo_rand() << 15;
    length = (length + pseudo_rand()) % 200000 + 1;
    for (int i = 0; i < length; ++i)
    {
        listValue[i] = pseudo_rand();
    }
    return length;
}

static bool run()
{
    int numQuery;

    int seed;
    int mIndex, mValue, mCopy, mLength;
    int userAns, ans;
    bool isCorrect = false;

    scanf("%d", &numQuery);

    for (int i = 0; i < numQuery; ++i)
    {
        int cmd;
        scanf("%d", &cmd);

        switch (cmd)
        {
        case CMD_INIT:
            init();
            isCorrect = true;
            break;
        case CMD_MAKE_LIST:
            scanf("%d", &seed);
            generateName(mName, seed);
            scanf("%d", &seed);
            mLength = generateList(mListValue, seed);
            makeList(mName, mLength, mListValue);
            break;
        case CMD_COPY_LIST:
            scanf("%d", &seed);
            generateName(mDest, seed);
            scanf("%d", &seed);
            generateName(mSrc, seed);
            scanf("%d", &mCopy);
            copyList(mDest, mSrc, (mCopy != 0));
            break;
        case CMD_UNDATE_ELEMENT:
            scanf("%d", &seed);
            generateName(mName, seed);
            scanf("%d %d", &mIndex, &mValue);
            updateElement(mName, mIndex, mValue);
            break;
        case CMD_ELEMENT:
            scanf("%d", &seed);
            generateName(mName, seed);
            scanf("%d", &mIndex);
            userAns = element(mName, mIndex);
            scanf("%d", &ans);
            if (userAns != ans)
            {
                isCorrect = false;
            }
            break;
        default:
            isCorrect = false;
            break;
        }
    }
    return isCorrect;
}

int main()
{
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}