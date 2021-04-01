#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define rint register int
#define MAX_ADD 40007
#define HASH_SIZE 20007
#define MAX_USER 1001

class package {
    public:
      //pos is the position of package in the heap array;
      int id, area, price, pos;
      package *next, *prev;
      void init(int i, int a, int p) {
          id = i; area = a; price = p; pos = -1;
      }
};
package *packages[MAX_ADD];

bool compare(package *a, package *b) {
    return a->price < b->price ? 1 : (a->price == b->price ? (a->id < b->id) : 0);
}
class Heap {
    public:
      int size;
      package *arr[MAX_ADD];

      void init() {
        size = 0;
      }
      void swap(int a, int b) {
        // Use the program data strucute and swap accordingly and update indices
        package *tmp = arr[a]; arr[a] = arr[b]; arr[b] = tmp;
        arr[a]->pos = a; arr[b]->pos = b;
      }

      void heapify(rint i) {
        rint l = i<<1, r = (i<<1) + 1, m = i;
        if (l <= size && compare(arr[l], arr[m])) m = l;
        if (r <= size && compare(arr[r], arr[m])) m = r;
        if (m != i) {
            swap(i, m);
            heapify(m);
        }
      }
      void insert(package *p) {
        rint i = ++size; //insert at the end
        arr[i] = p;
        p->pos = i;
        propagateUp(i);
      }
      void propagateUp(rint i) {
        while (i > 1 && compare(arr[i], arr[i>>1])) {
            swap(i>>1, i);
            i >>= 1;
        }
      }
      void remove(package *p) {
        rint i = p->pos;
        swap(i, size--); // put last item in the remove item place;
        propagateUp(i); // why are we doing this?
        heapify(i); // heapify the heap
      }
      package *top() {
        if (size > 0) {
            return arr[1];
        }
        return 0;
      }
};
Heap heap[11];

class HashMap { // this is a DLL
    public:
      package *head;
      void init() {
        head = 0;
      }
      void add(package *p) {
        if (head) {
          p->next = head;
          head->prev = p;
        }
        head = p;
      }
      void remove(package *p) {
        if (p == head) {
          head = head->next;
        } else {
          if (p->next) p->next->prev = p->prev;
          if (p->prev) p->prev->next = p->next;
        }
      }
      package* find(rint id) {
        package *curr = head;
        while(curr) {
          if(curr->id == id) {
            return curr;
          }
          curr = curr->next;
        }
        return 0;
      }
};
HashMap hashmap[HASH_SIZE];

int ith;
int once = 1;
package *base;
int friends[MAX_USER][22];
int book[MAX_USER][11];

void init(int N, int M) {
  if (once) {
    once = 0;
    base = new package();
    for (rint i = 0; i < MAX_ADD; i++) packages[i] = new package();
  }
  ith = 0;
  for (rint i = 0; i < MAX_USER; i ++) {
    friends[i][0] = 1;
    friends[i][1] = i;
    for (rint j = 0; j < 11; j++) book[i][j] = 0;
  }
  for (rint i = 0; i < HASH_SIZE; i++) hashmap[i].init();
  for (rint i = 0; i < 11; i++) heap[i].init();
}

void befriend(int uid1, int uid2) {
  friends[uid1][++friends[uid1][0]] = uid2;
  friends[uid2][++friends[uid2][0]] = uid1;
}

void add(int pid, int area, int price) {
  package *p = packages[ith++];
  p->init(pid, area, price);
  rint id = pid%HASH_SIZE; // not needed maybe?
  hashmap[id].add(p);
  heap[area].insert(p);
}

void reserve(int uid, int pid) {
  rint id = pid%HASH_SIZE;
  package *p = hashmap[id].find(pid);
  if (p) {
    heap[p->area].remove(p);
    hashmap[id].remove(p);
    book[uid][p->area]++;
  }
}

int recommend(int uid) {
  rint count[11] = { 0 };
  for (rint i = 1; i <= friends[uid][0]; i++) {
    for (rint j = 0; j < 11; j++) {
      count[j] += book[ friends[uid][i] ][j];
    }
  }
  base->init(2e9, -1, 1e5);
  rint m = 0;
  for (rint i = 1; i < 11; i++) if (count[i] > m) m = count[i];
  bool found = false;
  while (!found) {
    for (rint i = 1; i < 11; i++) {
      if (count[i] == m) {
        package *tmp = heap[i].top();
        if(tmp && compare(tmp, base)) {
          found = true;
          base->init(tmp->id, tmp->area, tmp->price);
        }
      }
    }
    m--;
  }
  return base->id;
}

// extern void init(int N, int M);
// extern void befriend(int uid1, int uid2);
// extern void add(int pid, int area, int price);
// extern void reserve(int uid, int pid);
// extern int  recommend(int uid);

#define INIT			100
#define BEFRIEND		200
#define ADD				300
#define RESERVE			400
#define RECOMMEND		500

static bool run()
{
	int L, N, M;
	bool okay = false;
	
	int pid, uid;
	int uid1, uid2;
	int area, price;
	
	int ans;
	int cmd, ret;

	scanf("%d", &L);

	for (int l = 0; l < L; ++l)
	{
		scanf("%d", &cmd);
		switch(cmd)
		{
		case INIT:
			scanf("%d %d", &N, &M);
			init(N, M);
			okay = true;
			break;
		case BEFRIEND:
			scanf("%d %d", &uid1, &uid2);
			befriend(uid1, uid2);
			break;
		case ADD:
			scanf("%d %d %d", &pid, &area, &price);
			add(pid, area, price);
			break;
		case RESERVE:
			scanf("%d %d", &uid, &pid);
			reserve(uid, pid);
			break;
		case RECOMMEND:
			scanf("%d %d", &uid, &ans);
			ret = recommend(uid);
			if (ret != ans)
				okay = false;
			break;
		}
	}
	
	return okay;
}

int main() {
	int TC, MARK;
	
    freopen("./sample_input.txt", "r", stdin);

	setbuf(stdout, NULL);
    scanf("%d %d", &TC, &MARK);

    for (int testcase = 1; testcase <= TC; ++testcase) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", testcase, score);
    }

    return 0;
}