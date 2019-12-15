/*
 * 
 * 
 */ 

#include <func.h>

#define MAX_TRIE_SIZE 128

typedef struct Trie{
    struct Trie* chTrie[MAX_TRIE_SIZE];
    bool m_flag;
} Trie;

/** Initialize your data structure here. */

Trie* trieCreate() {
    Trie *root = (Trie*)calloc(1, sizeof(Trie));
    root->m_flag = false;
    return root;

}

/** Inserts a word into the trie. */
void trieInsert(Trie* obj, char * word) {
    if(NULL == obj || NULL == word)
    {
        return;

    }

    Trie* cur = obj;
    while('\0' != *word)
    {
        if(NULL == cur->chTrie[(int)*word])
        {    
            cur->chTrie[(int)*word] = trieCreate();

        }
        cur = cur->chTrie[(int)*word];
        ++word;

    }
    cur->m_flag = true;

}

/** Returns if the word is in the trie. */
bool trieSearch(Trie* obj, char * word) {
    if(NULL == obj || NULL == word)
    {
        return false;

    }
    Trie *cur = obj;
    while('\0' != *word)
    {
        if(NULL == cur->chTrie[(int)*word])
        {
            return false;

        }
        cur = cur->chTrie[(int)*word];
        ++word;

    }

    return cur->m_flag;

}

/** Returns if there is any word in the trie that starts with the given prefix. */
bool trieStartsWith(Trie* obj, char * prefix) {
    if(NULL == obj || NULL == prefix)
    {
        return false;

    }

    Trie *cur = obj;
    while('\0' != *prefix)
    {
        if(NULL == cur->chTrie[(int)*prefix])
        {
            return false;

        }
        cur = cur->chTrie[(int)*prefix];
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


