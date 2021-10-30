#include <iostream>
#include <unordered_map>

using namespace std;

int fib(int n, int memo[])
{
    if (memo[n])
        return memo[n];
    if (n <= 2)
        return 1;

    memo[n] = fib(n - 1, memo) + fib(n - 2, memo);
    return memo[n];
}

int fibo(int n)
{
    if (n <= 2)
        return 1;
    return fibo(n - 1) + fibo(n - 2);
}

void testFib()
{
    int memo[51] = {
        0,
    };
    for (int i = 0; i < 50; i++)
    {
        cout << "fib" << i << " : " << fib(i, memo) << "  " << fibo(i) << endl;
    }
}

unsigned int gridTraveller(int r, int c)
{
    if (r == 1 && c == 1)
        return 1;
    if (r == 0 || c == 0)
        return 0;

    return gridTraveller(r - 1, c) + gridTraveller(r, c - 1);
}

unsigned int gridTravellerMemo(int r, int c, unordered_map<string, unsigned int> memo)
{
    string key = r + "," + c;

    if (memo.find(key) != memo.end())
        return memo[key];

    if (r == 1 && c == 1)
        return 1;
    if (r == 0 || c == 0)
        return 0;

    memo[key] = gridTravellerMemo(r - 1, c, memo) + gridTravellerMemo(r, c - 1, memo);
    return memo[key];
}

void testGridTraveller()
{
    unordered_map<string, unsigned int> memo;

    // cout << "(1,1): " << gridTraveller(1, 1) << endl;
    // cout << "(2,3): " << gridTraveller(2, 3) << endl;
    // cout << "(3,2): " << gridTraveller(3, 2) << endl;
    // cout << "(3,3): " << gridTraveller(3, 3) << endl;
    // cout << "(18,18): " << gridTraveller(18, 18) << endl;
    memo.clear();

    cout << "memo (1,1): " << gridTravellerMemo(1, 1, memo) << endl;
    cout << "memo (2,3): " << gridTravellerMemo(2, 3, memo) << endl;
    cout << "memo (3,2): " << gridTravellerMemo(3, 2, memo) << endl;
    cout << "memo (3,3): " << gridTravellerMemo(3, 3, memo) << endl;
    cout << "memo (18,18): " << gridTravellerMemo(18, 18, memo) << endl;
}

int main()
{
    //testFib();

    testGridTraveller();
}