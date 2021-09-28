#include <iostream>

#define MODULO 100
int dupCheck[MODULO] = {
    0,
};

#define INPUT 20
int input[INPUT];

#define MAX_NODES 100

using namespace std;

struct node
{
    int idx;
    int val;

    struct node *next;
    struct node *prev;
};
int nIdx = 0;
node Nodes[MAX_NODES];
node *Head = 0;
node *Tail = 0;
node *getNode()
{
    return &Nodes[nIdx++];
}

// return true if needs to be inserted;
// false if it needs to be go to next item;
bool isLess(int v1, int v2)
{
    if (v1 < v2)
    {
        return true;
    }
    return false;
}

int addN(int val, int idx)
{

    struct node *temp = getNode();
    temp->val = val;
    temp->idx = idx;
    temp->next = 0;
    temp->prev = 0;

    if (Head == 0)
    { // a. empty list;
        Head = temp;
        Tail = temp;
        return 0;
    }

    // check head;
    if (isLess(val, Head->val))
    { // b. add to head
        temp->next = Head;
        Head->prev = temp;
        Head = temp;
        return 0;
    }

    struct node *curr = Head;
    while (curr->next != 0 && (isLess(val, curr->next->val) == false))
    {
        curr = curr->next;
    }
    // c:: 1. assign next
    temp->next = curr->next;
    // 2. if not last, assign the prev
    if (curr->next != 0)
    { // not the last item;
        curr->next->prev = temp;
    }
    // do the rest; 4 lines code;
    curr->next = temp;
    temp->prev = curr;

    return 0;
}

int removeN(int val)
{
    if (Head->val == val)
    {
        Head = Head->next;
        Head->prev = 0;
        return 0;
    }

    struct node *curr = Head;
    while (curr != 0 && curr->val != val)
    {
        curr = curr->next;
    }
    if (curr->prev)
    {
        curr->prev->next = curr->next;
    }
    if (curr->next)
    {
        curr->next->prev = curr->prev;
    }
    return 0;
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

int main()
{

    createInput();
    // for (int i = 0; i < INPUT; i++) {
    //     addN(input[i], i);
    // }
    addN(4, 4);
    addN(6, 6);
    addN(0, 0);
    addN(2, 2);
    addN(3, 3);
    addN(5, 5);
    addN(7, 7);
    addN(1, 1);

    struct node *curr = Head;
    while (curr != 0)
    {
        cout << " " << curr->val;
        curr = curr->next;
    }
    cout << endl;

    removeN(2);
    removeN(7);
    removeN(1);
    removeN(0);
    removeN(5);

    curr = Head;
    while (curr != 0)
    {
        cout << " " << curr->val;
        curr = curr->next;
    }
    cout << endl;
    return 0;
}
