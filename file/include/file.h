/* welcome to world
 * file : .h
 * brief : 
 * author : BirdFSS
 * 
 * change log : 2019/12/15 create this file
 * 
 * 
 */ 

#ifndef __FILE_H__
#define __FILE_H__

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

#define FSTAT(fd, pst) { memset(pst, 0, sizeof(struct stat));\
            int ret = fstat(fd, pst); if(-1 == ret) {perror("fstat");} }
/*************************** STRUCT ****************************************/

/*************************** TYPEDEF *********************************************/

/*************************** GLOBAL FUNCTION *************************************/











#ifdef __cplusplus
}
#endif

#endif
