#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define INPUTWORD (100)
#define RECOMMEND (200)
#define BANWORD (300)

const int MAX_WORDS = 25000;
const int MAX_NODES = 500001;
const int STR_LEN = 20;
const int ALPHABET = 26;

int mstrcpy(char dst[], const char src[])
{
    int c = 0;
    while ((dst[c] = src[c]) != 0)
        ++c;
    dst[c] = 0;
    return c;
}

int mstrlen(const char str[])
{
    int ret = 0;
    while (str[ret])
        ++ret;
    return ret;
}

int mstrcmp(const char str1[], const char str2[])
{
    int c = 0;
    while (str1[c] != 0 && str1[c] == str2[c])
        ++c;
    return str1[c] - str2[c];
}

struct word
{
    char w[20];
    int freq;
    int ban;
    int idx;
    int ts;

    struct word *prev;
    struct word *next;
};
word Words[MAX_WORDS];
int wordCnt = 0;
int timestamp = 0;
word *newWord(char mWord[20])
{
    word *n = &Words[wordCnt++];
    n->ban = 0;
    n->freq = 1;
    n->ts = timestamp;
    n->idx = wordCnt - 1;
    mstrcpy(n->w, mWord);

    n->prev = 0;
    n->next = 0;

    return n;
};

struct Hash
{
    word *head;

    void init()
    {
        head = 0;
    }

    void add(word *n)
    {
        if (head)
        {
            n->next = head;
            head->prev = n;
        }
        head = n;
    }

    word *find(char mWord[20])
    {
        word *curr = head;
        while (curr)
        {
            if (mstrcmp(curr->w, mWord) == 0)
            {
                return curr;
            }
            curr = curr->next;
        }
        return 0;
    }

    void removeHash(word *n)
    {
        if (head == n)
        {
            head = head->next;
            head->prev = 0;
            return;
        }
        if (n->prev)
        {
            n->prev->next = n->next;
        }
        if (n->next)
        {
            n->next->prev = n->prev;
        }
    }
};
Hash wordMap[MAX_WORDS];

unsigned long getHash(char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c % MAX_WORDS;
    return hash % MAX_WORDS;
}

struct trie
{
    int eow;
    int mostFreqIdx;

    struct trie *child[26];
    struct trie *parent;
};
trie TRIE[MAX_NODES];
int trieCnt = 0;
trie *newTrie()
{
    trie *n = &TRIE[trieCnt++];
    n->eow = 0;
    n->mostFreqIdx = -1;

    for (int i = 0; i < 26; i++)
    {
        n->child[i] = 0;
    }
    n->parent = 0;
    return n;
}
trie *root;

bool compare(int idx1, int idx2)
{ // child, parent
    if (Words[idx1].ban == true)
    {
        return false;
    }
    if (Words[idx2].ban == true)
    {
        return false;
    }
    if (Words[idx1].freq > Words[idx2].freq)
    {
        return true;
    }
    if (Words[idx1].freq < Words[idx2].freq)
    {
        return false;
    }
    if (Words[idx1].ts > Words[idx2].ts)
    {
        return true;
    }
    return false;
}

void insertT(char mWord[20], int wordIdx)
{
    trie *curr = root;

    for (int i = 0; mWord[i] != '\0'; i++)
    {
        int offset = mWord[i] - 'a';
        if (curr->child[offset] == 0)
        {
            curr->child[offset] = newTrie();
        }
        curr->child[offset]->parent = curr;
        if (compare(wordIdx, curr->child[offset]->mostFreqIdx)) //update child mostFreqIdx with parent Idx
            curr->child[offset]->mostFreqIdx = wordIdx;
        curr = curr->child[offset];
    }
    curr->eow = 1;
}

trie *searchT(char mWord[20])
{
    trie *curr = root;
    for (int i = 0; mWord[i] != '\0'; i++)
    {
        int offset = mWord[i] - 'a';
        if (curr->child[offset] == 0)
        {
            return 0;
        }
        curr = curr->child[offset];
    }
    return curr;
}

void UpdateParent(trie *node, int banIdx)
{
    if (node == 0)
        return;

    node->mostFreqIdx = -1;
    int i = 0;
    for (; i < 26; i++)
    {
        if (node->child[i] != 0)
        {
            if (node->child[i]->mostFreqIdx == -1)
                continue;
            node->mostFreqIdx = node->child[i]->mostFreqIdx;
            break;
        }
    }
    for (; i < 26; i++)
    {
        if (node->child[i] != 0)
        {
            if (node->child[i]->mostFreqIdx == -1)
                continue;
            if (compare(node->child[i]->mostFreqIdx, node->mostFreqIdx))
            {
                node->mostFreqIdx = node->child[i]->mostFreqIdx;
            }
        }
    }

    if (node->parent && node->parent->mostFreqIdx == banIdx)
    {
        UpdateParent(node->parent, banIdx);
    }
}

int updatebanword(char mWord[20], int banIdx)
{
    trie *curr = root;
    for (int i = 0; mWord[i] != '\0'; i++)
    {
        int offset = mWord[i] - 'a';
        if (curr->child[offset] == 0)
        {
            return -1;
        }
        curr = curr->child[offset];
    }
    if (banIdx == curr->mostFreqIdx)
    {
        UpdateParent(curr, banIdx);
    }
    return curr->mostFreqIdx;
}

void init()
{
    wordCnt = 0;
    timestamp = 0;
    trieCnt = 0;
    for (int i = 0; i < MAX_WORDS; i++)
    {
        wordMap[i].init();
    }
    root = newTrie();
}

void inputWord(char mWord[20])
{
    timestamp++;
    word *n = 0;
    unsigned long h = getHash(mWord);

    n = wordMap[h].find(mWord);
    if (n)
    {
        if (!n->ban)
        {
            n->freq++;
            n->ts = timestamp;
        }
    }
    else
    {
        n = newWord(mWord);
        wordMap[h].add(n);
    }

    if (!n->ban)
    {
        insertT(mWord, n->idx);
    }
}

int recommend(char mUser[20], char mAnswer[20])
{
    trie *n = searchT(mUser);
    if (n == 0 || Words[n->mostFreqIdx].ban)
    {
        mstrcpy(mAnswer, mUser);
        return mstrlen(mAnswer);
    }
    mstrcpy(mAnswer, Words[n->mostFreqIdx].w);
    return mstrlen(mAnswer);
}

void banWord(char mWord[20])
{
    unsigned long h = getHash(mWord);
    word *n = wordMap[h].find(mWord);
    if (n)
    {
        n->ban = 1;
    }
    else
    {
        n = newWord(mWord);
        n->ban = 1;
        wordMap[h].add(n);
    }

    updatebanword(mWord, n->idx);
}

// static int mstrcmp(const char str1[], const char str2[])
// {
//     int c = 0;
//     while (str1[c] != 0 && str1[c] == str2[c])
//         ++c;
//     return str1[c] - str2[c];
// }

// static int mstrlen(const char str[])
// {
//     int ret = 0;
//     while (str[ret])
//         ++ret;
//     return ret;
// }

static int run()
{
    int ret_val = 1;
    int type, len;
    char mWord[20] = {}, mAnswer[20] = {}, ans[20] = {};

    init();

    int N;
    scanf("%d", &N);

    for (int i = 0; i < N; ++i)
    {
        scanf("%d %s", &type, mWord);

        switch (type)
        {
        case INPUTWORD:
            inputWord(mWord);
            break;
        case RECOMMEND:
            len = recommend(mWord, mAnswer);
            scanf("%s", ans);

            if (len != mstrlen(ans))
                ret_val = 0;
            if (mstrcmp(ans, mAnswer) != 0)
                ret_val = 0;
            break;
        case BANWORD:
            banWord(mWord);
            break;
        default:
            ret_val = 0;
            break;
        }
    }

    return ret_val;
}

int main()
{
    setbuf(stdout, NULL);
    freopen("input30.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}
