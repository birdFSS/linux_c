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
/* fn getFilePart
 * brief 将从文件偏移offset处开始的len个字节，读入缓冲区buf中
 * param[in] fd 文件描述符
 * param[out] buf 缓冲区
 * param[in] offset 偏移量
 * param[in] len 读取字节长度
 */
void getFilePart(int fd, char* buf, ssize_t offset, ssize_t len);











#ifdef __cplusplus
}
#endif

#endif
