#pragma once
#include"fun_all.h"
typedef struct tag_node{
    char ip[60];
    char port[8];
    char filePathAndName[256];
    int flag;
    struct tag_node *pNext;
}Node_t, *pNode_t;

typedef struct{
    pNode_t queHead, queTail;
    int capacity;
    int size;
    pthread_mutex_t mutex;
}Que_t, *pQue_t;

void queInit(pQue_t pq, int capacity);
void queInsert(pQue_t pq, pNode_t pnew);
int queGet(pQue_t pq, pNode_t *p);
void queDestroy(pQue_t pq);
