#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <unordered_map>

using namespace std;

#define INPUTWORD (100)
#define RECOMMEND (200)
#define BANWORD (300)

void mstrcpy(char dst[], const char src[])
{
    int c = 0;
    while ((dst[c] = src[c]) != 0)
        ++c;
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

const int MAX_WORDS = 25000;
const int MAX_NODES = 500001;
const int STR_LEN = 20;
const int ALPHABET = 26;

struct Word
{
    char word[STR_LEN];
    int freq;
    int index;
    bool is_banned;
    int time_stamp;

    Word *next;
    Word *prev;
};

Word words[MAX_WORDS];
int word_cnt;
int ts = 0;

unordered_map<string, Word *> wordMap;

struct Trie
{
    bool is_word;
    int index;
    Trie *parent;
    Trie *child[ALPHABET];
};
Trie nodes[MAX_NODES];
int trie_counter;
Trie *root;

Trie *getNode()
{
    Trie *t = &nodes[trie_counter];
    t->index = trie_counter;
    t->is_word = false;
    t->parent = 0;
    for (int i = 0; i < ALPHABET; i++)
    {
        t->child[i] = 0;
    }
    return t;
}

void insert(char mWord[20], int word_index)
{
    Trie *curr = root;
    for (int i = 0; mWord[i] != '\0'; i++)
    {
        int idx = mWord[i] - 'a';
        if (curr->child[idx] == 0)
        {
            curr->child[idx] = getNode();
        }
        curr->child[idx].parent = curr;
        // here there is a link between child and parent;
        curr = curr->child[idx];
    }
}

void UpdateParent(Trie *curr, int ban_index)
{
    if (curr == 0) // this is root
    {
        return;
    }

    root->index = -1;

    if (root->parent->index == ban_index) // ban_index
    {
    }
}

int searchT(char mWord[20], int ban_index)
{
    Trie *curr = root;
    for (int i = 0; mWord[i] != 0; i++)
    {
        int idx = mWord[i] - 'a';
        if (curr->child[idx] == 0)
        {
            return -1;
        }
        curr = curr->child[idx];
    }
    return curr->index;
}

void init()
{
    ts = 0;
    trie_counter = 0;
    word_cnt = 0;
    wordMap.clear();

    root = getNode();
    trie_counter++;
}

void inputWord(char mWord[20])
{
    ts++; // why ts?
    unordered_map<string, Word *>::iterator it = wordMap.find(string(mWord));
    if (it != wordMap.end())
    {
        Word *w = &(*it->second);
        if (w->is_banned == false)
        {
            w->freq++;
            w->time_stamp = ts;
            //add to trie for searching efficiently;
        }
    }
    else
    {
        words[word_cnt].freq = 0;
        words[word_cnt].index = word_cnt;
        words[word_cnt].is_banned = false;
        words[word_cnt].next = 0;
        words[word_cnt].prev = 0;
        words[word_cnt].time_stamp = ts;
        mstrcpy(words[word_cnt].word, mWord);
        wordMap[string(mWord)] = &words[word_cnt];

        word_cnt++;
        // add to trie for searching efficiently;
    }
}

int recommend(char mUser[20], char mAnswer[20])
{
    return 0;
}

void banWord(char mWord[20])
{
    unordered_map<string, Word *>::iterator it = wordMap.find(string(mWord));
    if (it != wordMap.end())
    {
        Word *w = &(*it->second);
        w->is_banned = true;
    }
    else
    {
        words[word_cnt].freq = 0;
        words[word_cnt].index = word_cnt;
        words[word_cnt].is_banned = true;
        words[word_cnt].next = 0;
        words[word_cnt].prev = 0;
        words[word_cnt].time_stamp = ts;
        mstrcpy(words[word_cnt].word, mWord);
        wordMap[string(mWord)] = &words[word_cnt];

        word_cnt++;
    }
    // parse the trie and ban this word;
}

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
    freopen("sample_input.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}
