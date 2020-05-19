/* welcome to world
 * file : main.c
 * brief : 
 * author : BirdFSS
 * 
 * change log : 2019/12/15 create this file
 * 
 * 
 */ 

/*************************** INCLUDE *********************************************/
#include "../include/trie.h"

/*************************** DEFINE **********************************************/

/*************************** LOCAL STRUCT ****************************************/

/*************************** TYPEDEF *********************************************/

/*************************** LOCAL FUNCTION **************************************/

/*************************** GLOBAL FUNCTION *************************************/

int main()  //int argc, char** argv)
{
    Trie* root = trieCreate();
    trieInsert(root, "hello");
    trieInsert(root, "hel");
    int i = 0;
    while(i < 100000000)
    {
        trieSearch(root, "hell");
        trieSearch(root, "hello");
        trieSearch(root, "hel");
        ++i;
    }
    trieFree(root);

    showMemery();

    return 0;
}

