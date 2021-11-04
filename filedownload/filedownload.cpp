#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// #include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>
#include <unordered_map>
#include <queue>

#define NAMELEN 11
#define PATHLEN 1001

#define CMD_MAKEDIR 100
#define CMD_MAKELINK 200
#define CMD_MAKESYSTEMFILE 300
#define CMD_FINDDOWNLOADDIR 400

static char path1[PATHLEN + 1];
static char path2[PATHLEN + 1];
static char name[NAMELEN + 1];

using namespace std;

#define MAX_FILES 10001

const string delim = "/";
const int SYM_LINK = 1;
const int SYS_FILE = 2;

const int MAXOBJ = 10001;

struct DIRINFO
{
    int safeTime;
    string name;
    list<DIRINFO *> nextTo;
};
DIRINFO dir[MAXOBJ];
int dirCnt = 0;

unordered_map<string, int> pathMap;

void init()
{
    dirCnt = 0;
    pathMap.clear();
    for (int i = 0; i < MAXOBJ; i++)
    {
        dir[i].nextTo.clear();
        dir[i].safeTime = MAXOBJ;
    }
    dir[dirCnt].name = delim;
    pathMap[delim] = dirCnt;
    dirCnt++;
}

void updateSafeTime(int idx)
{
    DIRINFO *que[MAXOBJ];
    que[0] = &dir[idx];

    int start = 0;
    int end = 1;

    while (true)
    {
        int curr = end;

        for (int i = start; i < end; i++)
        {
            DIRINFO *pDir = que[i];
            for (auto it = pDir->nextTo.begin(); it != pDir->nextTo.end(); it++)
            {
                DIRINFO *cDir = *it;
                if (cDir->safeTime > pDir->safeTime + 1)
                {
                    cDir->safeTime = pDir->safeTime + 1;
                    que[curr] = cDir;
                    curr++;
                }
            }
        }
        if (end == curr)
            break;

        start = end;
        end = curr;
    }
}

// asdf
void updateSafeTimeQ(int idx)
{
    queue<DIRINFO *> q;
    q.push(&dir[idx]);

    while (!q.empty())
    {
        DIRINFO *pDir = q.front();
        q.pop();
        for (auto it = pDir->nextTo.begin(); it != pDir->nextTo.end(); it++)
        {
            DIRINFO *cDir = *it;
            if (cDir->safeTime > pDir->safeTime + 1)
            {
                cDir->safeTime = pDir->safeTime + 1;
                q.push(cDir);
            }
        }
    }
}

void makeDir(char path[], char dirname[])
{
    int pathIdx = pathMap[(string)path];

    dir[dirCnt].name = (string)path + dirname + delim;
    dir[dirCnt].safeTime = dir[pathIdx].safeTime + 1;

    pathMap[(string)path + dirname + delim] = dirCnt;

    dir[dirCnt].nextTo.push_back(&dir[pathIdx]);
    dir[pathIdx].nextTo.push_back(&dir[dirCnt]);

    dirCnt++;
}

void makeLink(char path1[], char path2[])
{
    int path1Idx = pathMap[path1];
    int path2Idx = pathMap[path2];

    dir[path2Idx].nextTo.push_back(&dir[path1Idx]);

    updateSafeTime(path2Idx);
}

void makeSystemFile(char path[])
{
    int pathIdx = pathMap[path];
    dir[pathIdx].safeTime = 0;

    updateSafeTime(pathIdx);
}

// safest directory path;
void findDownloadDir(char downloadPath[])
{
    int maxIdx = -1;
    int maxSafeTime = -1;
    for (int i = 0; i < dirCnt; i++)
    {
        if (maxSafeTime < dir[i].safeTime)
        {
            maxSafeTime = dir[i].safeTime;
            maxIdx = i;
        }
    }
    strcpy(downloadPath, dir[maxIdx].name.c_str());
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int run(int Result)
{
    int N;
    scanf("%d", &N);

    init();

    for (int i = 0; i < N; i++)
    {
        int cmd;
        scanf("%d", &cmd);

        switch (cmd)
        {
        case CMD_MAKEDIR:
            scanf("%s%s", path1, name);
            makeDir(path1, name);
            break;

        case CMD_MAKELINK:
            scanf("%s%s", path1, path2);
            makeLink(path1, path2);
            break;

        case CMD_MAKESYSTEMFILE:
            scanf("%s", path1);
            makeSystemFile(path1);
            break;

        case CMD_FINDDOWNLOADDIR:
            path1[0] = 0;
            findDownloadDir(path1);
            bool isanswer = false;
            int ansnum;
            scanf("%d", &ansnum);
            for (int ans = 0; ans < ansnum; ans++)
            {
                scanf("%s", path2);
                if (strcmp(path1, path2) == 0)
                    isanswer = true;
            }
            if (!isanswer)
                Result = 0;
            break;
        }
    }
    return Result;
}

int main()
{
    freopen("sample_input.txt", "r", stdin);

    setbuf(stdout, NULL);

    int T, Result;
    scanf("%d %d", &T, &Result);

    for (int tc = 1; tc <= T; tc++)
    {
        printf("#%d %d\n", tc, run(Result));
    }
    return 0;
}