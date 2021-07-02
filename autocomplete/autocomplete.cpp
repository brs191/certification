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
 
struct Word {
    char word[STR_LEN];
    int freq;
    int index;
    bool is_banned;
    int time_stamp;
    Word* next;
    Word* prev;
 
};
 
Word words[MAX_WORDS];
int word_cnt;
int ts;

// if true then change
bool Compare(int idx1, int idx2) {
    if (words[idx1].is_banned == true)
        return false;
    if (words[idx2].is_banned == true)
        return true;
    if (words[idx1].freq > words[idx2].freq)
        return true;
    if (words[idx1].freq < words[idx2].freq)
        return false;
 
    if (words[idx1].time_stamp > words[idx2].time_stamp)
        return true;
    else
        return false;
}
 
Word* getWord(char word[STR_LEN], int ts) {
    Word* temp = &words[word_cnt++];
    mstrcpy(temp->word, word);
    temp->index = word_cnt - 1;
    temp->is_banned = false;
    temp->next = 0;
    temp->prev = 0;
    temp->freq = 1;
    temp->time_stamp = ts;
    return temp;
}
 
struct Hash {
    Word* head;
 
    void init() {
        head = 0;
    }
 
    void addWord(Word* node) {
        if (head) {
            node->next = head;
            head->prev = node;
        }
        head = node;
    }
 
    void removeWord(Word *node) {
        if (head == node) {
            head = head->next;
            return;
        }
 
        if (node->prev)
            node->prev->next = node->next;
        if (node->next)
            node->next->prev = node->prev;
    }
 
    Word* searcWord(char word[STR_LEN]) {
        Word* curr = head;
        while (curr) {
            if (mstrcmp(curr->word, word) == 0) {
                return curr;
            }
            curr = curr->next;
        }
        return 0;
    }
};
 
Hash ht[MAX_WORDS];
unsigned long getHash(char* str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c % MAX_WORDS;
    return hash % MAX_WORDS;
}
 
 
struct Trie {
    bool is_word;
    int index;
    Trie* parent;
    Trie* child[ALPHABET];
};
 
Trie nodes[MAX_NODES];
int trie_counter;
 
Trie* getNode() {
    Trie* temp = &nodes[trie_counter++];
    temp->is_word = false;
    temp->index = -1;
    for (register int i = 0; i < 26; i++) {
        temp->child[i] = 0;
    }
    temp->parent = 0;
    return temp;
}
 
Trie* root;
 
void insert(char mWord[20], int word_index) {
    Trie* curr = root;
    register int i = 0;
    while (mWord[i] != 0) {
        int index = mWord[i] - 'a';
        if (curr->child[index] == 0) {
            curr->child[index] = getNode();
        }
        curr->child[index]->parent = curr;
        if (true == Compare(word_index, curr->child[index]->index))
            curr->child[index]->index = word_index;
        curr = curr->child[index];
        i++;
    }
}
 
void UpdateParent(Trie* root, int ban_index) {
    if (root == 0) {
        return;
    }
 
    root->index = -1;
    register int i = 0;
    for (; i < ALPHABET; i++) {
            if (root->child[i] != 0) {
                if (root->child[i]->index == -1)
                    continue;
                root->index = root->child[i]->index;
                break;
            }
    }
 
    for (; i < ALPHABET; i++) {
            if (root->child[i] != 0) {
                if (root->child[i]->index == -1) {
                    continue;
                }
                if (true == Compare(root->child[i]->index, root->index)) {
                    root->index = root->child[i]->index;
                }
            }
    }
    if(root->parent && root->parent->index == ban_index)
        UpdateParent(root->parent, ban_index);
}
 
int search(char mWord[20]) {
 
    Trie* curr = root;
    register int i = 0;
    while (mWord[i] != 0) {
        int idx = mWord[i] - 'a';
        if (curr->child[idx] == 0)
            return -1;
        curr = curr->child[idx];
        i++;
    }
    return curr->index;
 
}

int updateBannedWord(char mWord[20], int ban_index) {
    Trie* curr = root;
    register int i = 0;
    while (mWord[i] != 0) {
        int idx = mWord[i] - 'a';
        if (curr->child[idx] == 0)
            return -1;
        curr = curr->child[idx];
        i++;
    }
 
    if (ban_index == curr->index) {
        UpdateParent(curr, ban_index);
    }
    return curr->index;
}

void init()
{
    word_cnt = 0;
    trie_counter = 0;
    ts = 0;
    for (register int i = 0; i < MAX_WORDS; i++) {
        ht[i].init();
    }
 
    root = getNode();
}
 
void inputWord(char mWord[20])
{
    ts++;
    int h = getHash(mWord);
    Word* word = ht[h].searcWord(mWord);
    if (word) {
        if (word->is_banned == false) {
            word->freq++;
            word->time_stamp = ts;
        }
    }
    else {
        word = getWord(mWord, ts);
        ht[h].addWord(word);
    }
    if (word->is_banned == false)
        insert(mWord, word->index);
}
 
int recommend(char mUser[20], char mAnswer[20])
{
    int index = search(mUser);
    if (index == -1 || words[index].is_banned == true) {
        int len = mstrcpy(mAnswer, mUser);
        return len;
    }
    int len = mstrcpy(mAnswer, words[index].word);
    return len;
}
 
void banWord(char mWord[20])
{
    int h = getHash(mWord);
    Word* word = ht[h].searcWord(mWord);
    if (word) {
        word->is_banned = true;
    } else {
        word = getWord(mWord, ts);
        word->is_banned = true;
        ht[h].addWord(word);
    }

    updateBannedWord(mWord, word->index);
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
    freopen("input30.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    printf("Read %d, %d", T, MARK);
    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}
