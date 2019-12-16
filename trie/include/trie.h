/* welcome to world
 * file : trie.h
 * brief : trie树
 * author : BirdFSS
 * 
 * change log : 2019/12/15 create this file
 * 
 * 
 */ 

#ifndef __TRIE_H__
#define __TRIE_H__

/*************************** INCLUDE *********************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

/*************************** DEFINE **********************************************/

#ifdef __cplusplus
extern "C"{
#endif

#define true 0
#define false -1

/*************************** STRUCT ****************************************/

struct Trie;
typedef struct Trie Trie;

/*************************** TYPEDEF *********************************************/
typedef int BOOL;

/*************************** GLOBAL FUNCTION *************************************/

void showMemery();
Trie* trieCreate();
void trieInsert(Trie* obj, const char * word);
BOOL trieSearch(Trie* obj, const char * word);
BOOL trieStartsWith(Trie* obj, const char * prefix);
void trieFree(Trie* obj);










#ifdef __cplusplus
}
#endif

#endif
