#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>


#define rint register int
#define MAX_PACKAGE 40009 // add function called 40,000 times
#define HASH_SIZE 20007
#define MAX_USERS 1001 // 1000 users
#define MAX_FRIENDS 20 // 20 friends
#define MAX_AREAS 11 // 10 areas a user can book


// user has friendscnt, friends list, and areas he booked;
typedef struct User {
	int frient_cnt;
	int friends[MAX_FRIENDS];
	int areas[MAX_AREAS];
} User;
User users[MAX_USERS];

static int M, N;


class package {
public:
	//pos is the position of package in the heap array;
	int id, area, price, pos;
	package* next, * prev;
	void init(int i, int a, int p) {
		id = i; area = a; price = p; pos = -1;
	}
};
package* packages[MAX_PACKAGE];

bool compare(package* a, package* b) {
	return a->price < b->price ? 1 : (a->price == b->price ? (a->id < b->id) : 0);
}
class Heap {
public:
	int size;
	package* arr[MAX_PACKAGE];

	void init() {
		size = 0;
	}
	void swap(int a, int b) {
		// Use the program data strucute and swap accordingly and update indices
		package* tmp = arr[a]; arr[a] = arr[b]; arr[b] = tmp;
		arr[a]->pos = a; arr[b]->pos = b;
	}

	void heapify(rint i) {
		rint l = i << 1, r = (i << 1) + 1, m = i;
		if (l <= size && compare(arr[l], arr[m])) m = l;
		if (r <= size && compare(arr[r], arr[m])) m = r;
		if (m != i) {
			swap(i, m);
			heapify(m);
		}
	}
	void insert(package* p) {
		rint i = ++size; //insert at the end
		arr[i] = p;
		p->pos = i;
		propagateUp(i);
	}
	void propagateUp(rint i) {
		while (i > 1 && compare(arr[i], arr[i >> 1])) {
			swap(i >> 1, i);
			i >>= 1;
		}
	}
	void remove(package* p) {
		rint i = p->pos;
		swap(i, size--); // put last item in the remove item place;
		propagateUp(i); // why are we doing this?
		heapify(i); // heapify the heap
	}
	package* top() {
		if (size > 0) {
			return arr[1];
		}
		return 0;
	}
};
Heap heap[11];

class HashMap { // this is a DLL
public:
	package* head;
	void init() {
		head = 0;
	}
	void add(package* p) {
		if (head) {
			p->next = head;
			head->prev = p;
		}
		head = p;
	}
	void remove(package* p) {
		if (p == head) {
			head = head->next;
		}
		else {
			if (p->next) p->next->prev = p->prev;
			if (p->prev) p->prev->next = p->next;
		}
	}
	package* find(rint id) {
		package* curr = head;
		while (curr) {
			if (curr->id == id) {
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
package* base;


void init(int n, int m) {
	N = n;
	M = m;
	if (once) {
		once = 0;
		base = new package();
		for (rint i = 0; i < MAX_PACKAGE; i++) packages[i] = new package();
	}
	ith = 0;
	for (rint i = 0; i < HASH_SIZE; i++) hashmap[i].init();
	for (rint i = 0; i < 11; i++) heap[i].init();
	for (register int i = 1; i <= N; i++) {
		users[i].frient_cnt = 0;
		for (register int j = 0; j <= M; j++) {
			users[i].areas[j] = 0;
		}
	}
}

void befriend(int uid1, int uid2) {
  // add uid1 <-> uid2
	users[uid1].friends[users[uid1].frient_cnt] = uid2;
	users[uid2].friends[users[uid2].frient_cnt] = uid1;
  // increase friend cnt
  users[uid1].frient_cnt++;
  users[uid2].frient_cnt++;
}

void add(int pid, int area, int price) {
	package* p = packages[ith++];
	p->init(pid, area, price);
	rint id = pid % HASH_SIZE; // not needed maybe?
	hashmap[id].add(p);
	heap[area].insert(p);
}

void reserve(int uid, int pid) {
	rint id = pid % HASH_SIZE;
	package* p = hashmap[id].find(pid);
	if (p) {
		heap[p->area].remove(p);
		hashmap[id].remove(p);
		users[uid].areas[p->area] += 1;
	}
}

int recommend(int uid) {
	rint uid_friends_areas[11] = { 0 };

  // get the areas reserved by the user uid
	for (register int i = 1; i <= M; i++) {
		uid_friends_areas[i] = users[uid].areas[i];
	}

  // get areas from all the friends; and accumulate them in tareas
	for (register int i = 0; i < users[uid].frient_cnt; i++) {
		int fId = users[uid].friends[i];
    // for each friendID get the areas cnt;
		for (register int j = 1; j <= M; j++) {
			uid_friends_areas[j] += users[fId].areas[j];
		}
	}

  // get package with highest bookings
  int max_booked_package = 0;
  package *pkg = 0;
  for (rint i = 1; i <= M; i++) {
    if(uid_friends_areas[i] > max_booked_package) max_booked_package = uid_friends_areas[i];
  }

  bool found = false;
  base->init(2e9, -1, 1e5);
  while (!found) {
    for (rint i = 1; i <= M; i++) {
	  // start looking for our desired package starting from max_booked_package and downwards;;
	  if (heap[i].size > 0 && uid_friends_areas[i] == max_booked_package) {
		package *top = heap[i].top();
		if(top && compare(top, base)) {
			found = true;
			base->init(top->id, top->area, top->price);
		}
	  }
    }
	max_booked_package--;
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
	
    // freopen("./input.txt", "r", stdin);
    freopen("./sample_input.txt", "r", stdin);

	  setbuf(stdout, NULL);
    scanf("%d %d", &TC, &MARK);

    for (int testcase = 1; testcase <= TC; ++testcase) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", testcase, score);
    }

    return 0;
}