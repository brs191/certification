#include <iostream>
#include <set>

using namespace std;

#define MAX_NODES 100
#define MODULO 100
int dupCheck[MODULO] = {
    -1,
};

#define INPUT 20
int input[INPUT];

struct node
{
    int val;
    int idx;
};
node Nodes[MAX_NODES];
int nodeIdx = 0;
node *getNode()
{
    return &Nodes[nodeIdx++];
}

void createInput()
{
    srand(time(NULL));
    int r = 0;

    for (int i = 0; i < INPUT; i++)
    {
        r = rand() % MODULO;
        while (dupCheck[r])
        {
            r = rand() % MODULO;
        }
        dupCheck[r] = r;
        input[i] = r;
    }

    for (int i = 0; i < INPUT; i++)
    {
        cout << input[i] << "\t";
    }
    cout << endl;
}

struct compare
{
    bool operator()(node *a, node *b)
    {
        if (b->val > a->val)
        {
            return true;
        }
        if (b->val == a->val)
        {
            if (b->idx > a->idx)
            {
                return true;
            }
        }
        return false;
    }
};
set<node *, compare> myset;

void setTest()
{
    node *temp = nullptr;
    for (int i = 0; input[i] != '\0'; i++)
    {
        temp = getNode();
        temp->idx = i;
        temp->val = input[i];
        myset.insert(temp);
    }
    temp = getNode();
    temp->idx = 999;
    temp->val = 999;
    myset.insert(temp);

    temp = getNode();
    temp->idx = 1;
    temp->val = 1;
    myset.insert(temp);

    for (auto it = myset.begin(); it != myset.end(); it++)
    {
        node *t = *it;
        cout << t->val << "\t";
    }
    cout << "\nset Test " << endl;
}

int main()
{
    createInput();
    setTest();
    cout << "end of stl" << endl;
}