#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

enum COMMAND
{
    CMD_ADD = 1,
    CMD_MOVE,
    CMD_INFECT,
    CMD_RECOVER,
    CMD_REMOVE
};

const int MAX_N = 20007;
const int _DIR = 100;
const int _FILE = 200;

struct Node
{
    int id;
    int type;
    int file_cnt;
    int file_size;
    int file_size_org;

    Node *parent;
    vector<Node *> dirs;
    vector<Node *> files;

    Node()
    {
        id = type = file_cnt = file_size = file_size_org = 0;
        parent = nullptr;
        dirs.clear();
        files.clear();
    }

    Node(int _id, int _fileSize)
    {
        id = _id;
        file_size = file_size_org = _fileSize;

        if (file_size == 0)
        {
            type = _DIR;
            file_cnt = 0;
        }
        else
        {
            type = _FILE;
            file_cnt = 1;
        }
        parent = nullptr;
        dirs.clear();
        files.clear();
    }

    void add(Node *&_new)
    {
        if (_new->type == _FILE)
        {
            files.push_back(_new);
        }
        else
        {
            dirs.push_back(_new);
        }
        _new->parent = this;
        refresh();
    }

    void erase(Node *&_child)
    {
        vector<Node *>::iterator itr = dirs.begin();
        while (itr != dirs.end())
        {
            Node *t = *itr;
            if (_child->id == t->id)
            {
                dirs.erase(itr);
                break;
            }
            itr++;
        }

        itr = files.begin();
        while (itr != files.end())
        {
            Node *t = *itr;
            if (_child->id == t->id)
            {
                files.erase(itr);
                break;
            }
            itr++;
        }

        _child->parent = nullptr;
        refresh();
    }

    void clear()
    {
        dirs.clear();
        files.clear();
        parent = nullptr;
        file_size = file_size_org = file_cnt = 0;
    }

    void infect(int changeSize)
    {
        if (this->type == _DIR)
        {
            for (int i = 0; i < files.size(); i++)
            {
                files[i]->file_size += changeSize;
            }

            for (int i = 0; i < dirs.size(); i++)
            {
                dirs[i]->infect(changeSize);
            }

            refresh();
        }
        else
        {
            file_size += changeSize;
            parent->refresh();
        }
    }

    void recover()
    {
        file_size = file_size_org;
        if (type == _DIR)
        {
            for (int i = 0; i < files.size(); i++)
            {
                files[i]->file_size = files[i]->file_size_org;
            }
            for (int i = 0; i < dirs.size(); i++)
            {
                dirs[i]->recover();
            }
            refresh();
        }
        else
        {
            parent->refresh();
        }
    }

    void refresh()
    {
        file_cnt = file_size = file_size = 0;
        for (int i = 0; i < dirs.size(); i++)
        {
            file_cnt += dirs[i]->file_cnt;
            file_size_org += dirs[i]->file_size_org;
            file_size += dirs[i]->file_size;
        }

        for (int i = 0; i < files.size(); i++)
        {
            file_cnt += 1;
            file_size_org += files[i]->file_size_org;
            file_size += files[i]->file_size;
        }

        if (parent != nullptr)
        {
            parent->refresh();
        }
    }
};
Node a[MAX_N];
int n;

unordered_map<int, Node *> map;

void init()
{
    map.clear();

    n = 0;
    a[n] = Node(10000, 0);

    map[10000] = &a[n];
    n += 1;
}

int cmdAdd(int newID, int pID, int fileSize)
{
    a[n] = Node(newID, fileSize);
    map[newID] = &a[n];

    Node *parent = map[pID];
    Node *curr = &a[n];
    parent->add(curr);

    n += 1;
    return parent->file_size;
}

int cmdMove(int tID, int pID)
{
    Node *curr = map[tID];
    curr->parent->erase(curr);

    Node *parent = map[pID];
    parent->add(curr);
    return parent->file_size;
}

int cmdRemove(int tID)
{
    Node *curr = map[tID];
    int ret = curr->file_size;

    if (tID == 10000)
    {
        curr->clear();
    }
    else
    {
        curr->parent->erase(curr);
    }
    return ret;
}

int cmdInfect(int tID)
{
    Node *root = map[10000];

    if (root->file_cnt == 0)
    {
        return 0;
    }

    int changeSize = root->file_size / root->file_cnt;
    Node *curr = map[tID];

    curr->infect(changeSize);
    return curr->file_size;
}

int cmdRecover(int tID)
{
    Node *curr = map[tID];
    curr->recover();

    return curr->file_size;
}

static int run(int score)
{
    int N;
    scanf("%d", &N);

    for (int i = 0; i < N; i++)
    {
        int cmd;
        int ret = 0;
        scanf("%d", &cmd);

        switch (cmd)
        {
        case CMD_ADD:
        {
            int id, pid, fileSize;
            scanf("%d%d%d", &id, &pid, &fileSize);
            ret = cmdAdd(id, pid, fileSize);
            break;
        }
        case CMD_MOVE:
        {
            int id, pid;
            scanf("%d%d", &id, &pid);
            ret = cmdMove(id, pid);
            break;
        }
        case CMD_INFECT:
        {
            int id;
            scanf("%d", &id);
            ret = cmdInfect(id);
            break;
        }
        case CMD_RECOVER:
        {
            int id;
            scanf("%d", &id);
            ret = cmdRecover(id);
            break;
        }
        case CMD_REMOVE:
        {
            int id;
            scanf("%d", &id);
            ret = cmdRemove(id);
            break;
        }
        }

        int checkSum;
        scanf("%d", &checkSum);
        if (ret != checkSum)
            score = 0;
    }
    return score;
}

int main()
{
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int TC, score;
    scanf("%d%d", &TC, &score);
    for (int t = 1; t <= TC; t++)
    {
        init();
        int ret = run(score);
        printf("#%d %d\n", t, ret);
    }

    return 0;
}