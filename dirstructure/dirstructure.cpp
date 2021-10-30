#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

using namespace std;

const int MAXOBJ = 10001;
const string delim = "/";

struct DIRINFO
{
    string path;
    list<DIRINFO *> nextTo;
};

DIRINFO dir[MAXOBJ];
int dirCnt;

unordered_map<string, int> pathMap;

void init()
{
    pathMap.clear();
    for (int i = 0; i < dirCnt; i++)
    {
        dir[i].nextTo.clear();
    }
    dir[0].path = delim;
    pathMap[delim] = 0; // 0th element is the parent;
    dirCnt = 1;         // root by default
}

void makedir(char path[], char dirname[])
{

    // 1. add new dirname with full path in Dir List
    dir[dirCnt].path = (string)path + dirname + delim;
    // 2. update pathMap of new dirname with full path with DirList index
    pathMap[dir[dirCnt].path] = dirCnt;

    // 3. add dirname address as the child to path;
    dir[pathMap[path]].nextTo.push_back(&dir[dirCnt]);

    // 4. add path i.e the parent to the nextTo list. (first element is the parent)
    dir[dirCnt].nextTo.push_back(&dir[pathMap[path]]);

    dirCnt++;
}

void makeLine(char path1[], char path2[])
{
    dir[pathMap[path2]].next.push_back(&dir[pathMap[path1]]);
}

void dirDFS(char path[])
{
    int pIdx = pathMap[path];
    DIRINFO *curr = 0;
    while (pIdx < dirCnt)
    {
        curr = &dir[pIdx];
        for (auto it = curr->nextTo.begin(); it != curr->nextTo.end(); it++)
        {
            DIRINFO *temp = *it;
            cout << temp->path << " ";
        }
        cout << endl;
        pIdx++;
    }
}

int main()
{
    init();

    makedir("/", "a");
    makedir("/", "b");
    makedir("/a/", "c");
    makedir("/b/", "d");
    makedir("/b/", "e");

    dirDFS("/");

    return 0;
}