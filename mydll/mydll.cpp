#include <iostream>

using namespace std;

int arr_idx = 0;

typedef struct _node
{
    int v;

    struct _node *prev;
    struct _node *next;
} NODE;

NODE a[20];
NODE *node_new(int val)
{
    NODE *self = &a[arr_idx];

    self->v = val;
    self->prev = NULL;
    self->next = NULL;
    arr_idx++;
    return self;
}

typedef struct _list
{
    NODE *head;
    NODE *tail;
    unsigned int len;
    void (*free)(void *val);
    int (*match)(void *a, int b);
} N_LIST;

N_LIST *list;

N_LIST *list_new(void)
{
    list->head = NULL;
    list->tail = NULL;
    list->free = NULL;
    list->match = NULL;
    list->len = 0;
    return list;
}

void list_node_free(void *_node)
{
    NODE *n = (NODE *)_node;
    n->v = -1;
    n->prev = NULL;
    n->next = NULL;
}

int list_match(void *a, int b)
{
    NODE *an = (NODE *)a;
    if (an->v == b)
        return 1;
    return 0;
}

NODE *list_rpush(N_LIST *self, NODE *node)
{
    if (self->len)
    {
        node->prev = self->tail;
        node->next = NULL;
        self->tail->next = node;
        self->tail = node;
    }
    else
    {
        self->head = self->tail = node;
        node->prev = node->next = NULL;
    }
    ++self->len;
    return node;
}

NODE *list_rpop(N_LIST *self)
{
    if (!self->len) // empty list
        return NULL;

    NODE *node = self->tail;
    if (--self->len)
    {
        // (self->tail = node->prev)->next = NULL;
        self->tail = node->prev;
        self->tail->next = NULL;
    }
    else
    {
        self->tail = self->head = NULL;
    }

    node->next = node->prev = NULL;
    return node;
}

NODE *list_lpush(N_LIST *self, NODE *node)
{
    if (self->len)
    {
        node->next = self->head;
        node->prev = NULL;
        self->head->prev = node;
        self->head = node;
    }
    else
    {
        self->head = self->tail = node;
        node->prev = node->next = NULL;
    }

    ++self->len;
    return node;
}

NODE *list_lpop(N_LIST *self)
{
    if (!self->len)
        return NULL;

    NODE *node = self->head;
    if (--self->len)
    {
        // (self->head = node->next)->prev = NULL;
        self->head = node->next;
        self->head->prev = NULL;
    }
    else
    {
        self->head = self->tail = NULL;
    }
    node->next = node->prev = NULL;
    return node;
}

void list_remove(N_LIST *self, NODE *node)
{
    if (node->prev)
    {
        node->prev->next = node->next;
    }
    else
    {
        self->head = node->next;
    }

    if (node->next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        self->tail = node->prev;
    }

    --self->len;
    if (self->free)
        self->free(node);
}

void list_addAfter(N_LIST *self, NODE *node, NODE *newNode)
{
    if (node->next)
    {
        node->next->prev = newNode;
        newNode->prev = node;

        newNode->next = node->next;
        node->next = newNode;
    }
    else
    {
        self->tail->next = newNode;
        newNode->prev = self->tail;
        self->tail = newNode;
    }
    ++self->len;
}

void list_addBefore(N_LIST *self, NODE *node, NODE *newNode)
{
    if (node->prev)
    {
        node->prev->next = newNode;
        newNode->prev = node->prev;

        newNode->next = node;
        node->prev = newNode;
    }
    else
    {
        self->head->prev = newNode;
        newNode->next = self->head;
        newNode->prev = NULL;
        self->head = newNode;
    }
    ++self->len;
}

NODE *list_find(N_LIST *self, int val)
{
    NODE *node = NULL;
    NODE *curr = self->head;
    while ((node = curr))
    {
        if (self->match(curr, val))
        {
            return node;
        }
        curr = curr->next;
    }
    return node;
}

void list_print()
{
    NODE *curr = list->head;
    for (int i = 0; i < list->len; i++)
    {
        cout << " " << curr->v;
        curr = curr->next;
    }
    cout << " ..\n";
}

int main()
{

    list = (N_LIST *)malloc(sizeof(N_LIST));
    list->free = list_node_free;
    list->match = list_match;

    list_print();
    list_rpush(list, node_new(10));
    list_print();
    list_rpush(list, node_new(21));
    list_print();

    list_lpush(list, node_new(41));
    list_print();
    list_lpush(list, node_new(0));
    list_print();

    NODE *found = list_find(list, 41);
    list_addAfter(list, found, node_new(52));
    list_print();

    list_addBefore(list, found, node_new(72));
    list_print();

    found = list_find(list, 0);
    list_addAfter(list, found, node_new(63));
    list_print();

    list_addBefore(list, found, node_new(65));
    list_print();

    list_remove(list, found);
    list_print();

    found = list_find(list, 21);
    list_addAfter(list, found, node_new(73));
    list_print();

    list_addBefore(list, found, node_new(75));
    list_print();

    list_remove(list, found);
    list_print();

    return 0;
}