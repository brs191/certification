#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

#define rint register int
#define MAX_NAMES 5011
#define MAX_LIST 2000001
#define SINGLE_LIST 200001

struct meta
{
    int listIdx;
    int len;
    int pos;
} Meta[MAX_NAMES];
int metaIdx = 0;
int LISTDATA[MAX_LIST];
int listIdx = 0;

unordered_map<string, int> makelist;
unordered_map<string, int> copylist;

unordered_map<string, int> referencelist;
unordered_map<string, vector<string>> references;

unordered_map<string, vector<pair<int, int>>> updates;
unordered_map<string, vector<pair<int, int>>> copyupdates;

void init()
{
    listIdx = 0;
    metaIdx = 0;
    for (auto itr = updates.begin(); itr != updates.end(); itr++)
    {
        itr->second.clear();
    }
}

void makeList(char mName[], int mLength, int mListValue[])
{
    int startOffset = listIdx;
    for (rint i = 0; i < mLength; i++)
    {
        LISTDATA[listIdx++] = mListValue[i];
    }
    Meta[metaIdx].pos = metaIdx;
    Meta[metaIdx].listIdx = listIdx - 1;
    Meta[metaIdx].len = mLength;

    makelist[mName] = metaIdx;
}

void copyList(char mDest[], char mSrc[], bool mCopy)
{
    if (mCopy)
    { // only values
        auto itr = makelist.find(mSrc);
        copylist[mDest] = itr->second;
        copyupdates[mDest] = updates[mSrc];
    }
    else
    { // reference
        auto itr = makelist.find(mSrc);

        referencelist[mDest] = itr->second;
        makelist[mDest] = itr->second;

        // reference list should have all the suggested modifications list;
        vector<string> names;
        names.push_back(mDest);
        // 1. check of mSrc exists in references;
        unordered_map<string, vector<string>>::iterator itrRS = references.find(mSrc);
        if (itrRS != references.end())
        {
            for (auto itr = itrRS->second.begin(); itr != itrRS->second.end(); itr++)
            {
                names.push_back(*itr);
            }
            references[mDest] = names;
        }
        else
        {
            names.push_back(mSrc);
            references[mDest] = names;
        }
        references[mSrc] = names;
    }
}

void updateElement(char mName[], int mIndex, int mValue)
{
    pair<int, int> update = {mIndex, mValue};

    auto itC = copylist.find(mName);
    if (itC != copylist.end())
    {
        auto itCUpdate = copyupdates.find(mName);
        if (itCUpdate != copyupdates.end())
        {
            itCUpdate->second.push_back(update);
        }
        else
        {
            vector<pair<int, int>> vitr;
            vitr.push_back(update);
            copyupdates[mName] = vitr;
        }
        return;
    }

    unordered_map<string, vector<pair<int, int>>>::iterator it = updates.find(mName);
    if (it == updates.end())
    { // vector is empty;
        // need to do something here;
        vector<pair<int, int>> vitr;
        vitr.push_back(update);
        updates[mName] = vitr;
    }
    else
    {
        it->second.push_back(update);
    }
}

int element(char mName[], int mIndex)
{
    auto itC = copylist.find(mName);
    if (itC != copylist.end())
    {
        unordered_map<string, vector<pair<int, int>>>::iterator itCU = copyupdates.find(mName);
        if (itCU != copyupdates.end())
        { // vector is empty;
            vector<pair<int, int>> update = itCU->second;
            bool found = false;
            for (auto i = update.begin(); i != update.end(); i++)
            {
                // 1. check if mIndex value is in the update table;
                // if it doesn't exit, return the original value from list;
                if (i->first == mIndex)
                {
                    found = true;
                    break;
                }
            }

            if (found == true)
            {   // we need to go through the modifications;
                // we need to go with the updates;
                auto itR = copylist.find(mName);
                int mIdx = 0;
                meta temp;
                if (itR != copylist.end())
                { // mName is in reference list;
                    mIdx = itR->second;
                    temp = Meta[mIdx];
                }
                int copyList[SINGLE_LIST];
                for (rint i = 0; i < temp.len; i++)
                {
                    copyList[i] = LISTDATA[temp.listIdx + i];
                }

                for (auto i = update.begin(); i != update.end(); i++)
                {
                    pair<int, int> tempPair = {0, 0};
                    tempPair.first = i->first;
                    tempPair.second = i->second;
                    copyList[i->first] = i->second;
                }
                return copyList[mIndex];
            }
            else
            {
                auto itR = copylist.find(mName);
                if (itR != copylist.end())
                { // mName is in reference list;
                    int mIdx = itR->second;
                    meta temp = Meta[mIdx];
                    return LISTDATA[temp.listIdx + mIndex];
                }
            }
        }
    }

    auto itRL = referencelist.find(mName);
    if (itRL != referencelist.end())
    {
        unordered_map<string, vector<pair<int, int>>>::iterator itU = updates.find(mName);
        if (itU != updates.end())
        { // vector is empty;
            vector<pair<int, int>> update = itU->second;
            bool found = false;
            for (auto i = update.begin(); i != update.end(); i++)
            {
                // 1. check if mIndex value is in the update table;
                // if it doesn't exit, return the original value from list;
                if (i->first == mIndex)
                {
                    found = true;
                    break;
                }
            }

            if (found == true)
            { // we need to go through the modifications;
                // we need to go with the updates;
                auto itR = referencelist.find(mName);
                int mIdx = 0;
                meta temp;
                if (itR != referencelist.end())
                { // mName is in reference list;
                    mIdx = itR->second;
                    temp = Meta[mIdx];
                }
                int copyList[SINGLE_LIST];
                for (rint i = 0; i < temp.len; i++)
                {
                    copyList[i] = LISTDATA[temp.listIdx + i];
                }

                for (auto i = update.begin(); i != update.end(); i++)
                {
                    pair<int, int> tempPair = {0, 0};
                    tempPair.first = i->first;
                    tempPair.second = i->second;
                    copyList[i->first] = i->second;
                }
                return copyList[mIndex];
            }
            else
            {
                auto itR = referencelist.find(mName);
                if (itR != referencelist.end())
                { // mName is in reference list;
                    int mIdx = itR->second;
                    meta temp = Meta[mIdx];
                    return LISTDATA[temp.listIdx + mIndex];
                }
            }
        }
    }

    // last check if mName is is make or copy list;
    auto it = makelist.find(mName);
    if (it != makelist.end())
    {
        int mIdx = it->second;
        meta temp = Meta[mIdx];
        return LISTDATA[temp.listIdx + mIndex];
    }
    return 0;
}