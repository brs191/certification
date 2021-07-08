#include <iostream>

/*
1. Copy Heap Code
2. Replace int with Package *
3. Cut and Copy while loop of HeapPush to SiftUp. Repalce while loop whith SiftUp
4. Copy while loop of HeapPop to SiftDown. Repalce while loop whith SiftDown
5. Update heap pos every where
6. Write Compare function.
7. Use Compare in the below 3 places
    1. while (current > 0 && Compare(heap[current], heap[(current - 1) / 2])) 
    
    2. child = Compare(heap[(current << 1) + 1], heap[(current << 1) + 2]) ? (current << 1) + 1 : (current << 1) + 2;
    3. if (Compare(heap[current], heap[child]))
*/
#define MODULO 1000
int dupCheck[MODULO];

#define MAX_SIZE 10
int input[MAX_SIZE];

struct node
{
    int val;
    int idx;
    int pos;

    struct node *prev;
    struct node *next;
};
node Nodes[MAX_SIZE];
int nodeCnt = 0;

node *newNode(int val)
{
    node *n = &Nodes[nodeCnt++];
    n->val = val;
    n->idx = nodeCnt - 1;
    n->pos = 0;

    n->prev = 0;
    n->next = 0;

    return n;
}

struct Heap
{
    node *heap[MAX_SIZE];
    int heapSize = 0;

    void heapInit(void)
    {
        heapSize = 0;
    }

    bool compare(node *n1, node *n2)
    {
        if (n1->val > n2->val)
            return true;
        if (n1->val < n2->val)
            return false;
        if (n1->idx > n2->idx)
            return true;
        return false;
    }

    int heapPush(node *value)
    {
        if (heapSize + 1 > MAX_SIZE)
        {
            return 0;
        }

        heap[heapSize] = value;
        value->pos = heapSize;
        ShiftUp(heapSize);
        heapSize = heapSize + 1;
        return 1;
    }

    void ShiftUp(int current)
    {
        while (current > 0 && compare(heap[current], heap[(current - 1) / 2])) // compare parent and child
        {
            node *temp = heap[(current - 1) / 2];
            heap[(current - 1) / 2] = heap[current];
            heap[(current - 1) / 2]->pos = (current - 1) / 2;
            heap[current] = temp;
            heap[current]->pos = current;
            current = (current - 1) / 2;
        }
    }

    int heapPop(node **value)
    {
        if (heapSize <= 0)
        {
            return -1;
        }

        *value = heap[0];
        heapSize = heapSize - 1;
        heap[0] = heap[heapSize];
        heap[0]->pos = 0;
        ShiftDown(0);
        return 1;
    }

    void ShiftDown(int current)
    {
        while (current * 2 + 1 < heapSize)
        {
            int child;
            if (current * 2 + 2 == heapSize)
            {
                child = current * 2 + 1;
            }
            else
            {
                child = compare(heap[current * 2 + 1], heap[current * 2 + 2]) ? current * 2 + 1 : current * 2 + 2;
            }

            if (compare(heap[current], heap[child]))
            {
                break;
            }

            node *temp = heap[current];
            heap[current] = heap[child];
            heap[current]->pos = current;
            heap[child] = temp;
            heap[child]->pos = child;

            current = child;
        }
    }

    node *Top()
    {
        return heap[0];
    }

    void deleteNode(node *val)
    {
        int idx = val->pos;
        heap[idx] = heap[heapSize--];
        heap[idx]->pos = idx;
        ShiftDown(idx);
        ShiftUp(idx);
    }

    void modifyNode(node *val)
    {
        ShiftUp(val->pos);
        ShiftDown(val->pos);
    }
};
Heap HEAP;

void insertionSort(void)
{
    int temp;
    int i;
    int j;

    for (i = 1; i < MAX_SIZE; i++)
    {
        temp = input[i];
        j = i - 1;

        while ((j >= 0) && (temp > input[j]))
        {
            input[j + 1] = input[j];
            j = j - 1;
        }
        input[j + 1] = temp;
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int r = rand() % MODULO;

    for (int i = 0; i < MODULO; i++)
    {
        dupCheck[i] = 0;
    }

    node *n = 0;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        r = rand() % MODULO;
        while (dupCheck[r])
        {
            r = rand() % MODULO;
        }
        dupCheck[r] = r;
        input[i] = r;
        n = newNode(r);
        HEAP.heapPush(n);
    }

    insertionSort();
    for (int i = 0; i < MAX_SIZE; i++)
    {
        HEAP.heapPop(&n);
        printf("%d:%d ", n->val, input[i]);
    }
    printf("\n");

    printf("Heap Code \n");
    return 0;
}
