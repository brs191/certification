#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

enum COMMAND
{
    CMD_ADD = 1,
    CMD_MOVE,
    CMD_INFECT,
    CMD_RECOVER,
    CMD_REMOVE
};

template <typename T>
struct vector
{
    T *arr;
    int sz;
    int capa;

    vector()
    {
        sz = 0;
        capa = 8;
        arr = new T[capa];
    }
    void clear()
    {
        sz = 0;
    }
    void resize(int _capa)
    {
        capa = _capa;

        T *tmp = new T[capa];

        for (int i = 0; i < sz; i++)
        {
            tmp[i] = arr[i];
        }
        delete arr;
        arr = tmp;
    }
    void push_back(T _new)
    {
        if (sz == capa)
        {
            resize(capa * 2);
        }

        arr[sz++] = _new;
    }
    void erase(T val)
    {
        for (int i = 0; i < sz; i++)
        {
            if (arr[i] == val)
            {
                arr[i] = arr[sz - 1];
                sz--;
                return;
            }
        }
    }
    T &operator[](int idx)
    {
        return arr[idx];
    }

    int size()
    {
        return sz;
    }

    T &back()
    {
        return arr[sz - 1];
    }
};

const int MAX_N = 20007;
template <typename KEY, typename VALUE>
struct unordered_map
{
    vector<KEY> *key;
    vector<VALUE> *value;
    int bucket_size;
    int (*get_key)(KEY);
    bool (*compare_key)(KEY, KEY);

    unordered_map()
    {
        bucket_size = 8;
        get_key = nullptr;
        compare_key = nullptr;
    }

    unordered_map(int (*_get_key)(KEY), bool (*_compare_key)(KEY, KEY), int BUCKET_SIZE)
    {
        bucket_size = BUCKET_SIZE;
        key = new vector<KEY>[bucket_size];
        value = new vector<VALUE>[bucket_size];
        get_key = _get_key;
        compare_key = _compare_key;
    }

    void clear()
    {
        for (int i = 0; i < bucket_size; i++)
        {
            key[i].clear();
            value[i].clear();
        }
    }

    VALUE &operator[](KEY _key)
    {
        int idx = get_key(_key) % bucket_size;

        for (int i = 0; i < key[idx].size(); i++)
        {
            if (compare_key(key[idx][i], _key))
                return value[idx][i];
        }

        key[idx].push_back(_key);
        VALUE _val = VALUE();
        value[idx].push_back(_val);

        return value[idx].back();
    }
};

const int _DIR = 100;
const int _FILE = 200;
struct Data
{
    int ID;
    int type;
    int file_cnt;
    int file_size_org;
    int file_size;
    Data *parent;
    vector<Data *> dirs;
    vector<Data *> files;

    Data()
    {
        ID = type = file_cnt = file_size_org = file_size = 0;
        parent = nullptr;
        dirs.clear();
        files.clear();
    }

    Data(int _ID, int _filesize)
    {
        ID = _ID;
        file_size = file_size_org = _filesize;
        if (_filesize == 0)
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
    void clear()
    {
        dirs.clear();
        files.clear();
        parent = nullptr;
        file_size = file_size_org = file_cnt = 0;
    }

    void add(Data *&_new)
    {
        if (_new->type == _DIR)
        {
            dirs.push_back(_new);
        }
        else
        {
            files.push_back(_new);
        }

        _new->parent = this;

        refresh();
    }

    void refresh()
    {
        file_cnt = file_size_org = file_size = 0;

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

    void erase(Data *&child)
    {
        dirs.erase(child);
        files.erase(child);

        child->parent = nullptr;

        refresh();
    }

    void infect(int add_size)
    {
        if (this->type == _DIR)
        {
            for (int i = 0; i < files.size(); i++)
            {
                files[i]->file_size += add_size;
            }

            for (int i = 0; i < dirs.size(); i++)
            {
                dirs[i]->infect(add_size);
            }

            refresh();
        }
        else
        {
            file_size += add_size;
            parent->refresh();
        }
    }

    void recover()
    {
        file_size = file_size_org;
        if (this->type == _DIR)
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
};

Data a[MAX_N];
int n;

int get_key_int(int val)
{
    return val;
}

bool compare_key_int(int lval, int rval)
{
    return lval == rval;
}

unordered_map<int, Data *> map = unordered_map<int, Data *>(get_key_int, compare_key_int, MAX_N);

void init()
{
    map.clear();
    n = 0;
    a[n] = Data(10000, 0);

    map[10000] = &a[0];
    n++;
}

int cmdAdd(int newID, int pID, int fileSize)
{
    a[n] = Data(newID, fileSize);
    map[newID] = &a[n];

    Data *parent = map[pID];
    Data *cur = &a[n];
    parent->add(cur);
    n++;

    return parent->file_size;
}

int cmdMove(int tID, int pID)
{
    Data *cur = map[tID];

    cur->parent->erase(cur);
    Data *parent = map[pID];

    parent->add(cur);

    return parent->file_size;
}

int cmdInfect(int tID)
{

    Data *root = map[10000];
    if (root->file_cnt == 0)
        return 0;

    int inc = root->file_size / root->file_cnt;

    Data *cur = map[tID];
    cur->infect(inc);
    return cur->file_size;
}

int cmdRecover(int tID)
{
    Data *cur = map[tID];

    cur->recover();

    return cur->file_size;
}

int cmdRemove(int tID)
{
    Data *cur = map[tID];
    int ret = cur->file_size;

    if (tID == 10000)
    {
        cur->clear();
    }
    else
    {
        cur->parent->erase(cur);
    }
    return ret;
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
