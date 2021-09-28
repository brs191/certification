#include <iostream>
#include <list>

using namespace std;

#define MAX_NODES 100
#define MODULO 100
int dupCheck[MODULO] = {
    0,
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

list<node *> mylist;
void listTest()
{
    for (int i = 0; input[i] != '\0'; i++)
    {
        node *temp = getNode();
        temp->idx = i;
        temp->val = input[i];
        mylist.push_back(temp);
    }
    mylist.pop_back();
    mylist.erase(mylist.begin());
}

int main()
{
    createInput();

    listTest();

    for (auto it = mylist.begin(); it != mylist.end(); it++)
    {
        node *t = *it;
        cout << t->val << "\t";
    }
    cout << "\nend of listStl" << endl;
    return 0;
}