#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>
#include <unordered_map>

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

struct dir
{
    int type;
    string path;
    list<dir *> next;
};
dir Dir[MAX_FILES];
int dirCnt = 0;

unordered_map<string, int> pathMap;

void init()
{
    dirCnt = 0;
}

void makeDir(char path[], char dirname[])
{
    int pathIdx = pathMap[path];
    int dirIdx = dirCnt;
    dirCnt++;

    Dir[dirIdx].path = (string)path + dirname + delim;
    pathMap[Dir[dirIdx].path] = dirIdx;
}

void makeLink(char path1[], char path2[])
{
}

void makeSystemFile(char path[])
{
}

void findDownloadDir(char downloadPath[])
{
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