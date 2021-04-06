#include <iostream>
#include <string.h>

#define rint register int
#define HEAP_SIZE 101

class Data {
    public:
        int freq;
        char word[21];
        int pos;
        void init() {
            freq = 0;
        }
        void init(char w[], int f) {
            freq = f;
            strcpy(word, w);
        }
};

bool compare(Data *d1, Data *d2) {
    return (d1->freq > d2->freq) ? true : false;
};

class heap {
    public:
        int size = 0;
        Data *arr[HEAP_SIZE];

        void init() {
            size = 0;
        }

        void swap(int i, int j) {
            Data *tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
            arr[i]->pos = i; arr[j]->pos = j;
        }

        void insert(Data *d) {
            int i = d->pos = ++size;
            arr[i] = d;
            propUp(i);
        }
        void propUp(int i) {
            while (i > 1 && compare(arr[i], arr[i >> 1])) {
                swap(i, i >> 1);
                i >>= 1;
            }
        }

        void remove(Data *d) {
            int i = d->pos;
            swap(i, size--);
            propUp(i);
            heapify(i);
        }

        void heapify(int i) {
            int m = i, l = (i << 1), r = (i << 1) + 1;
            if (l <= size && compare(arr[l], arr[m])) m = l;
            if (r <= size && compare(arr[r], arr[m])) m = r;
            if (m != i) {
                swap(i, m);
                heapify(m);
            }
        }

        Data *top() {
            if (size > 0) {
                return arr[1];
            }
            return 0;
        }
};

heap Heap[HEAP_SIZE];

#define DATA_SIZE 10
Data *entries[DATA_SIZE];

int once = 1;
int ith = 0;
int main() {

    if (once) {
        once = 0;
        for (rint i = 0; i < DATA_SIZE; i++) entries[i] = new Data();
    }
    ith = 0;
    for (rint i = 0; i < HEAP_SIZE; i++) Heap[i].init();

    // insert data 1
    Data *d = entries[ith++];
    char w[] = "hello";
    d->init(w, 1);
    Heap[1].insert(d);

    // insert data 1
    d = entries[ith++];
    char b[] = "bello";
    d->init(b, 2);
    Heap[1].insert(d);

    // insert data 1
    d = entries[ith++];
    char g[] = "gulli";
    d->init(g, 33);
    Heap[1].insert(d);

    Data *t = Heap[1].top();
    printf("top is %s, %d \n", t->word, t->freq);

    d = entries[ith++];
    char c[] = "culli";
    d->init(c, 43);
    Heap[1].insert(d);

    t = Heap[1].top();
    printf("top is %s, %d \n", t->word, t->freq);

    d = entries[ith++];
    char tu[] = "tulli";
    d->init(tu, 23);
    Heap[1].insert(d);

    t = Heap[1].top();
    printf("top is %s, %d \n", t->word, t->freq);

    d = entries[ith++];
    char z[] = "zulli";
    d->init(z, 223);
    Heap[1].insert(d);

    t = Heap[1].top();
    printf("top is %s, %d \n", t->word, t->freq);

    Heap[1].remove(d);
    printf("remove zulli \n");
    
    t = Heap[1].top();
    printf("top is %s, %d \n", t->word, t->freq);

    printf("Heap implementation complete \n");
    return 0;
}