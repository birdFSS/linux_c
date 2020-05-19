/* welcome to world
 * file : .c
 * brief : 
 * author : BirdFSS
 * 
 * change log : 2019/12/15 create this file
 * 
 * 
 */ 

/*************************** INCLUDE *********************************************/
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

/*************************** DEFINE **********************************************/
#define BUF_SIZE 4096

/*************************** LOCAL STRUCT ****************************************/

/*************************** TYPEDEF *********************************************/

/*************************** LOCAL FUNCTION **************************************/

/*************************** GLOBAL FUNCTION *************************************/

int write_file(int srcfd, int destfd)
{
    int ret = 0;
    char buf[BUF_SIZE] = {0};
    while(((ret = read(srcfd, buf, BUF_SIZE)) > 0))
    {
        if(-1 == write(destfd, buf, ret))
        {
            perror("write");
            exit(1);
        }
    }

    return 0;
}

int mmap_write_file(int srcfd, int destfd)
{
    char* addr = NULL, *daddr=NULL;
    struct stat st;
    memset(&st, 0, sizeof(struct stat));

    fstat(srcfd, &st);

    addr = (char*)mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, srcfd, 0);
    if(MAP_FAILED == addr)
    {
        perror("mmap");
        exit(1);
    }

    if(-1 == ftruncate(destfd, st.st_size))
    {
        perror("ftruncate");
        exit(1);
    }

    daddr = (char*)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, destfd, 0);
    if(MAP_FAILED == daddr)
    {
        perror("mmap");
        exit(1);
    }

    memcpy(daddr, addr, st.st_size);
    munmap(addr, st.st_size);
    munmap(daddr, st.st_size);
    return 0;
}



int main(/*int argc, char** argv*/)
{
    int srcfd = open("test.txt", O_RDONLY);
    int destfd1 = 0;
    int destfd2 = 0;
    char filepath1[256] = {0};
    char filepath2[256] = {0};
    int i = 0;
    while(i++ < 1000)
    {
#if 1
        sprintf(filepath1, "./tmp/%s%d", "file", i);
        destfd1 = open(filepath1, O_RDWR | O_CREAT, 0666);
        if(-1 == destfd1)
        {
            perror("open");
            return -1;
        }
        write_file(srcfd, destfd1);
        lseek(srcfd, 0, SEEK_SET);
        close(destfd1);
#else 
        sprintf(filepath2, "./tmp/%s%d", "mfile", i);
        destfd2 = open(filepath2, O_RDWR | O_CREAT, 0666);
        if(-1 == destfd2)
        {
            perror("open");
            printf("***");
            return -1;
        }
        mmap_write_file(srcfd, destfd2);
        close(destfd2);
#endif
        
    }
    return 0;
}


















