#include <stdio.h>

/*
Tip 1:
Arrays can be used instead of dynamic assignment
*/

int arr_idx = 0;

typedef struct _node {
    int v;

    struct _node *prev;
    struct _node *next;
} NODE;
NODE a[10000];

NODE *myalloc(void) {
    return &a[arr_idx++];
}

NODE *plist = NULL;
NODE *rear = NULL;

void insert(int val) {
    NODE *p = myalloc();
    p->v = val;
    p->next = NULL;
    p->prev = NULL;

    if(plist == NULL) {
        plist = rear = p;
    } else {
        p->next = plist;
        plist->prev = p;
        plist = p;
    }
}

void append(int val) {
    NODE *p = myalloc();
    p->v = val;
    p->next = NULL;
    p->prev = NULL;
    if(plist == NULL) { // 1st node
        plist = p;
    } else {
        rear->next = p;
        p->prev = rear;   
    }
    rear = p;
}

void removeFront() {
    if (plist != NULL) {
        plist = plist->next;
    }
    if(plist != NULL)
        plist->prev = NULL;

    if(plist == NULL)
        rear = plist;
}

void removeRear() {
    if(rear != NULL) {
        if(rear->prev == NULL) {
            plist = rear = NULL;
        } else {    
            rear->prev->next = NULL;
            rear = rear->prev;
        }
    }
}

void print() {
    for (NODE *pp = plist; pp != NULL; pp = pp->next) {
        printf("%d ", pp->v);
    }
    printf("\n");
}

int main() {
    arr_idx = 0;
    for(int i = 0; i < 10; i ++) {
        append(i);
        print();
    }

    for(int i = 0; i < 10; i++) {
        removeRear();
        print();
    }

    for(int i = 0; i < 10; i ++) {
        insert(i);
        print();
    }

    for(int i = 0; i < 10; i++) {
        removeFront();
        print();
    }
    return 0;
}