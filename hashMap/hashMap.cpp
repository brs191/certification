#include <iostream>

#define rint register int

#define MODULO 1000
#define DUPLICATE 1000

#define INPUT_SIZE 100

int dupCheck[MODULO];
int input[INPUT_SIZE];

#define TOPK 7
int result[TOPK];
int resultCnt = 0;

void insertionSort(int val)
{
	int temp;
	int i;
	int j;

    if(resultCnt < TOPK) {
        result[resultCnt++] = val;
    } else if(result[TOPK-1] > val) {
        result[TOPK-1] = val;
    }

	for (i = 1; i < TOPK; i++)
	{
        if(result[i] == -1) break;

		temp = result[i];
		j = i - 1;

		while ((j >= 0) && (temp < result[j]))
		{
			result[j + 1] = result[j];
			j = j - 1;
		}
		result[j + 1] = temp;
	}
}

struct node {
    int val;
    int pos;

    struct node *prev;
    struct node *next;
};
node Nodes[INPUT_SIZE];
int nodeCnt = 0;
node *getNode(int val) {
    node *n = &Nodes[nodeCnt++];
    n->val = val;
    n->pos = 0;

    n->prev = 0;
    n->next = 0;
}

struct heap {
    int size = 0;
    node *arr[INPUT_SIZE];

    void init() {
        size = 0;
    }

    int swap(int i, int j) {
        node *t = arr[i]; arr[i] = arr[j]; arr[j] = t;
        arr[i]->pos = i;
        arr[j]->pos = j;
    }
    
    bool compare(node *a, node *b) {
        if (a->val < b->val) return true;
        return false;
    }

    void add(node *n) {
        int i = n->pos = ++size;
        arr[i] = n;
        Propup(i);
    }

    void remove(node *n) {
        int i = n->pos;
        swap(i, size--);
        Propup(i);
        Heapify(i);
    }

    void Propup(int i) {
        while(i > 1 && compare(arr[i], arr[i >> 1])) {
            swap(i, i >> 1);
            i = i >> 1;        }
    }

    void Heapify(int i) {
        int m = i, l = (i << 1), r = (i << 1) +1;

        if(l <= size && compare(arr[l], arr[m])) m = l;
        if(r <= size && compare(arr[r], arr[m])) m = r;

        if(m != i) {
            swap(i, m);
            Heapify(m);
        }
    }

    node *TOP() {
        return arr[1];
    }
};
heap HEAP;

int main() {
    srand(time(NULL));
    int r = rand() % MODULO;

    resultCnt = 0;
    for(rint i = 0; i < TOPK; i++) {
        result[i] = -1;
    }

    HEAP.init();

    // for(rint i = 0; i < INPUT_SIZE; i++) {
    //     input[i] = 0;
    // }

    for(rint i = 0; i < INPUT_SIZE; i++) {
        r = rand() % MODULO;
        while(dupCheck[r]) {
            r = rand() % MODULO;
        }
        dupCheck[r] = r;
        input[i] = r;
        node *n = getNode(r);
        
        insertionSort(r);
        HEAP.add(n);
    }

    for(rint i = 0; i < resultCnt; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    printf("TOP is %d\n", HEAP.TOP()->val);

    printf("return main\n");
    return 0;
}