#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

// H2129 File Cache
// #define NULL 0
#define MAX_NODE 20000
#define MAX_HASH_SIZE 30000

struct Node
{
    int id, pos, size;
    Node *prev;
    Node *next;
    Node *up;
    Node *down;
    Node *fprev;
    Node *fnext;
};

Node NodePool[MAX_NODE];
int NodePoolCnt, Usage;
Node *Head;
Node *Top;
Node *Bottom;
Node *FreeHead;

struct HashId
{
    int key;
    Node *data;
};
HashId HashIdTbl[MAX_HASH_SIZE];

Node *findId(int key)
{
    unsigned long h = key % MAX_HASH_SIZE;
    int cnt = MAX_HASH_SIZE;

    while (HashIdTbl[h].key != -1 && cnt--)
    {
        if (HashIdTbl[h].key == key)
        {
            return HashIdTbl[h].data;
        }
        h = (h + 1) % MAX_HASH_SIZE;
    }
    return NULL;
}

void addId(int key, Node *data)
{
    unsigned long h = key % MAX_HASH_SIZE;

    while (HashIdTbl[h].key != -1)
    {
        if (HashIdTbl[h].key == key)
        {
            HashIdTbl[h].data = data;
            return;
        }
        h = (h + 1) % MAX_HASH_SIZE;
    }

    HashIdTbl[h].key = key;
    HashIdTbl[h].data = data;
}

Node *newNode(int id, int size)
{
    Node *ret = &NodePool[NodePoolCnt++];
    ret->id = id;
    ret->size = size;
    ret->prev = ret->next = ret->up = ret->down = ret->fprev = ret->fnext = NULL;
    return ret;
}

void add(Node *node)
{
    Usage += node->size;
    if (Bottom)
    {
        Bottom->down = node;
        node->up = Bottom;
        node->down = NULL;
        Bottom = node;
    }
    else
    {
        Top = Bottom = node;
        node->up = NULL;
        node->down = NULL;
    }
}

void remove(Node *node)
{
    Usage -= node->size;
    Node *up = node->up;
    Node *down = node->down;

    if (up)
        up->down = down;
    else
        Top = down;

    if (down)
        down->up = up;
    else
        Bottom = up;
}

Node *pop()
{
    Node *delNode = Top;
    Usage -= delNode->size;
    if (Top)
    {
        Top = Top->down;
        if (Top == NULL)
            Bottom = Top;
        else
            Top->up = NULL;
    }

    addId(delNode->id, NULL);
    Node *left = delNode->prev;
    Node *right = delNode->next;
    if (left && left->id < 0)
    {
        left->size += delNode->size;
        if (right && right->id < 0)
        {
            left->size += right->size;
            left->next = right->next;
            if (right->next)
            {
                right->next->prev = left;
            }

            left->fnext = right->fnext;
            if (right->fnext)
            {
                right->fnext->fprev = left;
            }
        }
        else
        {
            left->next = right;
            if (right)
            {
                right->prev = left;
            }
        }
        return left;
    }
    else if (right && right->id < 0)
    {
        right->pos = delNode->pos;
        right->size += delNode->size;
        right->prev = left;
        if (left)
        {
            left->next = right;
        }
        else
        {
            Head = right;
        }
        return right;
    }

    while (left && left->id >= 0)
        left = left->prev;
    while (right && right->id >= 0)
        right = right->next;

    if (left)
    {
        left->fnext = delNode;
    }
    else
    {
        FreeHead = delNode;
    }

    if (right)
    {
        right->fprev = delNode;
    }

    delNode->fprev = left;
    delNode->fnext = right;
    delNode->id = -1;
    return delNode;
}

int allocate(Node *curr, int fileId, int fileSize)
{
    if (curr->size > fileSize)
    {
        Node *node = newNode(fileId, fileSize);
        node->pos = curr->pos;
        curr->pos += fileSize;
        curr->size -= fileSize;
        node->next = curr;
        if (curr->prev)
        {
            curr->prev->next = node;
        }
        else
        {
            Head = node;
        }
        node->prev = curr->prev;
        curr->prev = node;
        addId(fileId, node);
        add(node);
        return node->pos;
    }
    else if (curr->size == fileSize)
    {
        curr->id = fileId;
        addId(fileId, curr);
        add(curr);

        if (curr->fprev)
        {
            curr->fprev->fnext = curr->fnext;
        }
        else
        {
            FreeHead = curr->fnext;
        }

        if (curr->fnext)
        {
            curr->fnext->fprev = curr->fprev;
        }

        return curr->pos;
    }

    return -1;
}

////////////////////////////////////////////////

void init(int N)
{
    for (int i = 0; i < MAX_HASH_SIZE; ++i)
    {
        HashIdTbl[i].key = -1;
    }

    NodePoolCnt = Usage = 0;
    Head = FreeHead = newNode(-1, N);
    Head->pos = 0;
    Top = Bottom = NULL;
}

int access(int fileId, int fileSize)
{
    Node *node = findId(fileId);
    if (node)
    {
        remove(node);
        add(node);
        return node->pos;
    }

    Node *curr = FreeHead;
    while (curr)
    {
        int ret = allocate(curr, fileId, fileSize);
        if (ret >= 0)
            return ret;

        curr = curr->fnext;
    }

    while (Top)
    {
        curr = pop();
        int ret = allocate(curr, fileId, fileSize);
        if (ret >= 0)
            return ret;
    }

    return -1;
}

int usage()
{
    return Usage;
}

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 1
#define CMD_ACCESS 2
#define CMD_USAGE 3

static bool run()
{
    int q;
    scanf("%d", &q);

    int n, fileId, fileSize;
    int cmd, ans, ret = 0;
    bool okay = false;

    for (int i = 0; i < q; ++i)
    {
        scanf("%d", &cmd);
        switch (cmd)
        {
        case CMD_INIT:
            scanf("%d", &n);
            init(n);
            okay = true;
            break;
        case CMD_ACCESS:
            scanf("%d %d %d", &fileId, &fileSize, &ans);
            ret = access(fileId, fileSize);
            if (ans != ret)
                okay = false;
            break;
        case CMD_USAGE:
            ret = usage();
            scanf("%d", &ans);
            if (ans != ret)
                okay = false;
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

    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}
