
#include <algorithm>
#include <list>
#include <unordered_map>

using namespace std;

#define MAX_N	(500)
#define MAX_P	(8)

struct RESULT
{
	int r, c;
};

int MAP[MAX_N][MAX_N];
const int MAX_HASH = 32769;
typedef pair<int, int> POINT;
POINT points[MAX_N * MAX_N];
int point_counter;

typedef unordered_map<int, list<POINT*>> HashMap;
HashMap HMAP;

struct Node {
	int r;
	int c;
	Node* next;
	Node* prev;
};

Node nodes[2500000];
int node_cnt;

Node* getNode(int r, int c) {
	Node* temp = &nodes[node_cnt++];
	temp->r = r;
	temp->c = c;
	temp->next = 0;
	temp->prev = 0;
	return temp;
}

int N;
int M;
struct PICS {
	int PIC[MAX_P][MAX_P];
};
PICS pics[4];

struct MyHash {
	Node* head;
	void init() {
		head = 0;
	}

	void addNode(Node* node) {
		if (head) {
			node->next = head;
			head->prev = node;
		}
		head = node;
	}
	Node* SearchNode(int r, int c) {
		Node* curr = head;
		while (curr) {
			if (curr->r == r && curr->c == c) {
				return curr;
			}
			curr = curr->next;
		}
		return 0;
	}

	void removeNode(Node* node) {
		if (head == node) {
			head = head->next;
			node->next = 0;
			node->prev = 0;
			node = 0;
			return;
		}
		if (node->prev) {
			node->prev->next = node->next;
		}
		if (node->next) {
			node->next->prev = node->prev;
		}
		node->next = 0;
		node->prev = 0;
		node = 0;

	}
};
MyHash myH[MAX_HASH];

bool isValid(int r, int c) {
	if (r >= 0 && r < N && c >= 0 && c < N)
		return true;
	return false;
}
void init(int N, int M, int mMap[MAX_N][MAX_N])
{
	::N = N;
	::M = M;
	point_counter = 0;
	for (register int i = 0; i < MAX_HASH; i++) {
		myH[i].init();
	}
	for (register int r = 0; r < N; r++) {
		for (register int c = 0; c < N; c++) {
			MAP[r][c] = mMap[r][c] + 16384; // Up the number;
			Node *t = getNode(r, c);
			myH[MAP[r][c]].addNode(t);
		}
	}
}

bool FindMatch(int r, int c, int pic_idx, int mValue, RESULT* ret) {
	if (mValue < 0 || mValue >= MAX_HASH) {
		return false;
	}
	Node* curr = myH[mValue].head;
	if (curr == 0)
		return false;
	while(curr) {
		int r1 = curr->r;
		int c1 = curr->c;
		int r2 = r1 + M - 1;
		int c2 = c1 + M - 1;
		if (false == isValid(r2, c2)) {
			curr = curr->next;
		}
		else {
			int y, x, m, n;
			if (abs(pics[pic_idx].PIC[0][0] - MAP[r1][c1]) < 256 &&
				abs(pics[pic_idx].PIC[0][M - 1] - MAP[r1][c2]) < 256 &&
				abs(pics[pic_idx].PIC[M - 1][0] - MAP[r2][c1]) < 256 &&
				abs(pics[pic_idx].PIC[M - 1][M - 1] - MAP[r2][c2]) < 256) {
				bool status = true;

				int k = -1;
				for (y = r1; y < r2; y++) {
					k++;
					int l = -1;
					for (x = c1; x < c2; x++) {
						l++;
						if (abs(pics[pic_idx].PIC[k][l] - MAP[y][x]) >= 256) {
							return false;
						}
					}
				}
				if (status == true) {
					ret->r = r1;
					ret->c = c1;
					return true;
				}
			}
			curr = curr->next;
		}
	}
	return false;
}

RESULT findPosition(int mPic[MAX_P][MAX_P])
{
	RESULT ret;
	ret.r = ret.c = -1;
	int cnt = 0;
	for (register int r = 0; r < M; ++r) {
		for (register int c = 0; c < M; ++c) {
			pics[0].PIC[r][c] = mPic[r][c] + +16384;
			pics[1].PIC[r][c] = mPic[M - 1 - c][r] + +16384;
			pics[2].PIC[r][c] = mPic[M - 1 - r][M - 1 - c] + 16384;
			pics[3].PIC[r][c] = mPic[c][M - 1 - r] + 16384;
		}
	}

	for (register int idx = 0; idx < 4; idx++) {
		for (register int i = 0; i < 256; i++) {
			bool status = false;
			ret.r = -1;
			ret.c = -1;
			int mValue = pics[idx].PIC[0][0] + i;
			status = FindMatch(0, 0, idx, mValue, &ret);
			if (status == true) {
				return ret;
			}
			mValue = pics[idx].PIC[0][0] - i;
			status = FindMatch(0, 0, idx, mValue, &ret);
			if (status == true) {
				return ret;
			}
		}
	}
	return ret;
}

void updateMap(int r, int c, int mNewValue)
{
	int val = MAP[r][c];
	Node *curr = myH[val].SearchNode(r, c);
	if (curr) {
		myH[val].removeNode(curr);
	}
	int mValue = mNewValue + 16384;
	MAP[r][c] = mValue;
	myH[mValue].addNode(getNode(r, c));
}