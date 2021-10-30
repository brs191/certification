#include <stdio.h>

#define MAX_SIZE 100

int heap[MAX_SIZE];
int heapSize = 0;

void heapInit(void)
{
    heapSize = 0;
}

void shiftUp(int current)
{
    while (current > 0 && heap[current] < heap[(current - 1) / 2])
    {
        int temp = heap[(current - 1) / 2];
        heap[(current - 1) / 2] = heap[current];
        heap[current] = temp;
        current = (current - 1) / 2;
    }
}

void shiftDown(int current)
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
            child = heap[current * 2 + 1] < heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
        }

        if (heap[current] < heap[child])
        {
            break;
        }

        int temp = heap[current];
        heap[current] = heap[child];
        heap[child] = temp;

        current = child;
    }
}

int heapPush(int value)
{
    if (heapSize + 1 > MAX_SIZE)
    {
        printf("queue is full!");
        return 0;
    }

    heap[heapSize] = value;

    int current = heapSize;
    shiftUp(heapSize);
    heapSize = heapSize + 1;

    return 1;
}

int heapPop(int *value)
{
    if (heapSize <= 0)
    {
        return -1;
    }

    *value = heap[0];
    heapSize = heapSize - 1;

    heap[0] = heap[heapSize];

    int current = 0;
    shiftDown(current);
    return 1;
}

int heapTop()
{
    return heap[0];
}
void heapModify(int pos)
{
    int idx = pos;
    shiftUp(idx);
    shiftDown(idx);
}

void heapDelete(int pos)
{
    heap[pos] = heap[--heapSize];
    shiftDown(pos);
    shiftUp(pos);
}

int N = 10;
int main(int argc, char *argv[])
{
    int input[] = {2, 5, 23, 56, 82, 70, 3, 7, 10, 22, 47};
    heapInit();

    int inputlen = sizeof(input) / sizeof(input[0]);
    for (int i = 0; i < inputlen; i++)
    {
        heapPush(input[i]);
    }

    for (int i = 0; i < inputlen; i++)
    {
        int value;
        heapPop(&value);
        printf("%d ", value);
    }
    printf("\nheap Done0\n");

    heapInit();

    for (int i = 0; i < inputlen; i++)
    {
        heapPush(input[i]);
    }

    printf("modify: %d\n", heap[6]);
    heap[6] = 77;
    heapModify(6);

    printf("delete: %d\n", heap[2]);
    heapDelete(2);

    printf("Heap Top is %d\n", heapTop());

    for (int i = 0; i < inputlen; i++)
    {
        int value;
        heapPop(&value);
        printf("%d ", value);
    }
    printf("\nheap Done1\n");

    return 0;
}
