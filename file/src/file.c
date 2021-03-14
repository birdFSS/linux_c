/* welcome to world
 * file : file.c
 * brief :  对文件进行一些处理操作
 * author : BirdFSS
 * 
 * change log : 2019/12/15 create this file
 * 
 * 
 */ 

/*************************** INCLUDE *********************************************/
#include "file.h"
/*************************** DEFINE **********************************************/
#define TEST_FILE 1

/*************************** LOCAL STRUCT ****************************************/

/*************************** TYPEDEF *********************************************/

/*************************** LOCAL FUNCTION **************************************/

/*************************** GLOBAL FUNCTION *************************************/

void getFilePart(int fd, char* buf, ssize_t offset, ssize_t len)
{
    struct stat st;
    int ret;
    FSTAT(fd, &st);
    if(offset > st.st_size || offset < 0 || len < 0)
    {
        return;
    }

    if(offset + len > st.st_size)
    {
        len = st.st_size - offset;
    }

    lseek(fd, offset, SEEK_SET);
    
    while(len > 0)
    {
        if(-1 == (ret = read(fd, buf, len)))
        {
            perror("read");
            return;
        }
        len -= ret;
    }
}


#if TEST_FILE
void test_getFilePart()
{
    int fd = open("./src/file.c", O_RDWR, 0766);
    char* buf = (char*)calloc(1, 65536);
    getFilePart(fd, buf,8,213);
    printf("---------------buf1-------------------\n%s\n", buf);

    memset(buf, 0, 65536);
    getFilePart(fd, buf,11111911, 122);
    printf("\n--------------buf2-------------------\n%s\n", buf);

    memset(buf, 0, 65536);
    getFilePart(fd, buf,0, 65536);
    printf("\n--------------buf3-------------------\n%s\n", buf);

    memset(buf, 0, 65536);
    getFilePart(fd, buf,100, -213);
    printf("\n--------------buf4-------------------\n%s\n", buf);
    free(buf);
    close(fd);
}

int main()
{
    test_getFilePart();
}

#endif
