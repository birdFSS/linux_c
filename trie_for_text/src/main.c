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
    printf("%d\n", trieSearch(root, "hell"));
    printf("%d\n", trieSearch(root, "hello"));
    printf("%d\n", trieSearch(root, "hel"));
    trieFree(root);

    showMemery();

    char test[100] = "符峰海";
    printf("%ld\n", strlen(test));

    return 0;
}

