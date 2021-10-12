#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <set>
#include <valarray>

using namespace std;
// https://github.com/gibsjose/cpp-cheat-sheet/blob/master/Data%20Structures%20and%20Algorithms.md
// https://www.logicalbee.in/cpp-stl-algorithms-cheat-sheet/#make_heap
// https://medium.com/logicalbee/c-stl-algorithms-cheat-sheet-d92f986abe14
// https://hackingcpp.com/cpp/cheat_sheets.html

void stringConversion()
{
    int num = 123459;
    string str = "123";
    char cstr[] = "234";

    cout << stoi(str) << endl;
    cout << atoi(cstr) << endl;
    cout << to_string(num) << endl;
}

void unOrderMap()
{
    unordered_map<string, string> mymap;
    mymap = {{"India", "New Delhi"}, {"U.S.", "Washington"}, {"France", "Paris"}};
    unordered_map<string, string>::iterator itr = mymap.begin();
    for (; itr != mymap.end(); itr++)
    {
        cout << itr->first << " :: " << itr->second << endl;
    }
    cout << "bucket count " << mymap.bucket_count() << endl;
}

// https://www.geeksforgeeks.org/sorting-a-vector-in-c/
struct ascending
{
    bool operator()(int N1, int N2)
    {
        if (N1 < N2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
} ascend;
bool Descending(int N1, int N2)
{
    if (N1 > N2)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void vectors()
{
    vector<int> myvector;
    for (int i = 0; i < 10; i++)
    {
        myvector.push_back(i);
    }

    sort(myvector.begin(), myvector.end(), Descending);
    sort(myvector.begin(), myvector.end(), ascend);

    vector<int>::iterator itr = myvector.begin();
    for (; itr != myvector.end(); itr++)
    {
        cout << " " << *itr;
    }
    cout << endl;
}

void permutations()
{
    string str = "dabc";
    // with out sort; Generates next permutation till it is not lexicographically largest
    sort(str.begin(), str.end());
    do
    {
        cout << str << endl;
    } while (next_permutation(str.begin(), str.end()));
}

//
struct setCompare
{
    bool operator()(int a, int b)
    {
        if (a > b)
            return true;
        return false;
    }
};

void setS()
{
    int myints[] = {75, 23, 65, 42, 13, 1};
    set<int, setCompare> mySet(myints, myints + 6);

    mySet.insert(191);

    set<int>::iterator itr = mySet.begin();
    for (; itr != mySet.end(); itr++)
    {
        cout << *itr << " ";
    }
    cout << endl;
}

vector<int> recursiveVector()
{
    vector<int> myvec;
    for (int i = 1; i <= 10; i++)
    {
        myvec.push_back(i);
    }
    return myvec;
}

void vectorTest()
{
    auto temp = recursiveVector();
    for (auto itr = temp.begin(); itr != temp.end(); itr++)
    {
        cout << *itr << " ";
    }
    cout << endl;
}

void sliceTest()
{
    valarray<int> myslice;
    // todo; ^^
}

int main()
{
    stringConversion();
    unOrderMap();
    vectors();
    permutations();
    setS();
    vectorTest();
    sliceTest();
}