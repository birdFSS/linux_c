/* welcome to world
 * file : trie.c
 * brief : trie树
 * author : BirdFSS
 * 
 * change log : 2019/12/15 create this file
 *              2019/12/15 first version
 * 
 */ 

/*************************** INCLUDE *********************************************/
#include "../include/trie.h"

/*************************** DEFINE **********************************************/
#define MAX_TRIE_SIZE 256
#define CH_OFFSET 128

/*************************** LOCAL STRUCT ****************************************/
struct Trie{
    struct Trie* chTrie[MAX_TRIE_SIZE];
    BOOL m_flag;
};
/*************************** TYPEDEF *********************************************/

/*************************** LOCAL VAL  ******************************************/
static size_t memCnt = 0;
/*************************** GLOBAL VAL ******************************************/

/*************************** LOCAL FUNCTION **************************************/

/*************************** GLOBAL FUNCTION *************************************/

/** Initialize your data structure here. */
Trie* trieCreate() {
    Trie *root = (Trie*)calloc(1, sizeof(Trie));
    root->m_flag = false;

    memCnt += sizeof(Trie);
    return root;
}

/** Inserts a word into the trie. */
void trieInsert(Trie* obj, const char * word) {
    if(NULL == obj || NULL == word)
    {
        return;
    }

    Trie* cur = obj;
    int tmp;
    while('\0' != *word)
    {
        tmp = (int)*word + CH_OFFSET;
        if(NULL == cur->chTrie[tmp])
        {    
            cur->chTrie[tmp] = trieCreate();
        }
        cur = cur->chTrie[tmp];
        ++word;
    }
    cur->m_flag = true;

}

/** Returns if the word is in the trie. */
BOOL trieSearch(Trie* obj, const char * word) {
    if(NULL == obj || NULL == word)
    {
        return false;
    }
    Trie *cur = obj;
    int tmp;
    while('\0' != *word)
    {
        tmp = (int)*word + CH_OFFSET;
        if(NULL == cur->chTrie[tmp])
        {
            return false;
        }
        cur = cur->chTrie[tmp];
        ++word;
    }
    return cur->m_flag;
}

/** Returns if there is any word in the trie that starts with the given prefix. */
BOOL trieStartsWith(Trie* obj, const char * prefix) {
    if(NULL == obj || NULL == prefix)
    {
        return false;
    }

    Trie *cur = obj;
    int tmp;
    while('\0' != *prefix)
    {
        tmp = (int)*prefix + CH_OFFSET;
        if(NULL == cur->chTrie[tmp])
        {
            return false;
        }
        cur = cur->chTrie[tmp];
        ++prefix;
    }

    return true;
}

void trieFree(Trie* obj) {
    if(NULL == obj)
    {
        return;
    }
    Trie * cur = obj;
    for(int i = 0; i < MAX_TRIE_SIZE; ++i)
    {
        if(cur->chTrie[i] != NULL)
        {
            trieFree(cur->chTrie[i]);
        }
    }
    free(cur);
}

void showMemery()
{
    printf("memery size = %ldMB %ldKB %ldB\n", memCnt /(1024 * 1024), memCnt / 1024 % 1024, memCnt % 1024);
    printf("node cnt = %ld\n", memCnt/sizeof(Trie));
}

