#include <stdio.h>
#include <algorithm>
#include <unordered_map>

using namespace std;

const int MAX_N = 500;
const int MAX_P = 8;
const int MAX_HASH = 2 ^ 15 + 1;

int MAP[MAX_N][MAX_N];

typedef pair<int, int> POINT;
POINT points[MAX_N * MAX_N];
int point_counter;

typedef unordered_map<int, list<POINT *>> HashMap;
HashMap HMAP;

int N;
int M;

struct RETURN
{
	int r;
	int c;
};

`struct PICS
{
	int PIC[MAX_P][MAX_P];
};
PICS pics[4];

bool isValid(int r, int c)
{
	if (r >= 0 && r < N && c >= 0 && c < N)
		return true;
	return false;
}

void init(int N, int M, int mMap[MAX_N][MAX_N])
{
	::N = N;
	::M = M;
	point_counter = 0;
	HMAP.clear();
	for (register int r = 0; r < N; r++)
	{
		for (register int c = 0; c < N; c++)
		{
			MAP[r][c] = mMap[r][c] + 16384; // Up the number;
			points[point_counter].first = r;
			points[point_counter].first = c;
			HMAP[MAP[r][c]].push_back(&points[point_counter++]);
		}
	}
}

bool FindMatch(int r, int c, int pic_idx, int noise_value, RETURN *ret)
{
	int mValue = pics[pic_idx].PIC[r][c] + noise_value;
	HashMap::iterator it = HMAP.find(mValue);
	if (it == HMAP.end())
	{
		return false;
	}

	for (auto it : it->second)
	{
		int r1 = it->first;
		int c1 = it->second;
		int r2 = r1 + M;
		int c2 = c1 + M;
		if (false == IsVailid(r2, c2) {
			continue;
		}
		int flag, y, x, m, n;
		if (abs(pics[pic_idx].PIC[0][0] - MAP[r1][c1]) < 256 &&
			abs(pics[pic_idx].PIC[0][M - 1] - MAP[r1][c2 - 1]) < 256 &&
			abs(pics[pic_idx].PIC[M - 1][0] - MAP[r2][c1]) < 256 &&
			abs(pics[pic_idx].PIC[M - 1][M - 1] - MAP[r2][c2]) < 256) {
			for (y = r1; y < r2; y++)
			{
				for (x = c1; x < c2; x++)
				{
					flag = 0;
					for (m = 0; m < M; m++)
					{
						for (n = 0; n < M; m++)
						{
							if (abs(pics[pic_idx].PIC[m][n] - MAP[y + m][x + n] >= 256))
							{
								flag = 1;
								break;
							}
						}
					}
					if (flag == 0)
					{
						ret->r = y + 1;
						ret->c = x + 1;
						return true;
					}
				}
				if (flag == 0)
				{
					ret->r = y + 1;
					ret->c = x + 1;
					return true;
				}
			}
		}
	}
	return false;
}

RETURN findPosition(int map[MAX_P][MAX_P])
{
	RETURN ret;
	ret.r = -1;
	ret.c = -1;
	int cnt = 0;
	//0 degree
	for (register int r = 0; r < M; r++)
	{
		for (register int c = 0; c < M; c++)
		{
			pics[cnt].PIC[r][c] = map[r][c];
		}
	}
	cnt++;
	// 90 Degrees :
	// Transpose
	for (register int r = 0; r < M; r++)
	{
		for (register int c = 0; c < M; c++)
		{
			pics[cnt].PIC[r][c] = map[c][r];
		}
	}
	// Swap Columns
	for (register int r = 0; r < M; r++)
	{
		for (register int c = 0; c < M / 2; c++)
		{
			int temp = pics[cnt].PIC[r][c];
			pics[cnt].PIC[r][c] = pics[cnt].PIC[r][M - c - 1];
			pics[cnt].PIC[r][M - c - 1] = temp;
		}
	}
	cnt++;
	//180 degrees Transform
	for (int r = 0; r < M / 2; r++)
	{
		for (int c = 0; c < M; c++)
		{
			pics[cnt].PIC[r][c] = map[M - r - 1][M - c - 1];
		}
	}
	cnt++;
	// 270 Degrees :
	// Transpose
	for (register int r = 0; r < M; r++)
	{
		for (register int c = 0; c < M; c++)
		{
			pics[cnt].PIC[r][c] = map[c][r];
		}
	}
	// Swap Columns
	for (register int r = 0; r < M; r++)
	{
		for (register int c = 0; c < M / 2; c++)
		{
			int temp = pics[cnt].PIC[r][c];
			pics[cnt].PIC[r][c] = pics[cnt].PIC[M - r - 1][c];
			pics[cnt].PIC[M - r - 1][c] = temp;
		}
	}

	for (register int i = 0; i < 256; i++)
	{
		for (register int idx = 0; idx < 4; i++)
		{
			bool status = false;
			ret.r = -1;
			ret.c = -1;
			status = FindMatch(0, 0, idx, i, &ret);
			if (status == true)
			{
				break;
			}
		}
	}
	return ret;
}

struct Compare
{
	int r_;
	int c_;
	Compare(int r, int c) : r_(r), c_(c){};
	bool operator()(POINT *p1)
	{
		if (r_ == p1->first && c_ == p1->second)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

void updateValue(int r, int c, int mValue)
{
	mValue = mValue + 16384;
	HashMap::iterator it = HMAP.find(mValue); // find mValue and remove r and c from here
	if (it != HMAP.end())
	{
		//Update MAP
		int pValue = MAP[r][c];
		MAP[r][c] = mValue;
		POINT p{r, c};
		list<POINT *>::iterator lit = find_if(it->second.begin(), it->second.end(), Compare(r, c));
		if (lit != it->second.end())
		{
			it->second.erase(lit);
		}
		points[point_counter].first = r;
		points[point_counter].first = c;
		HMAP[mValue].push_back(&points[point_counter++]);
	}
}

int main()
{

	init() return 0;
}