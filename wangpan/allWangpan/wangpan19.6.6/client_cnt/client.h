#include "fun_all.h"
#define TEST_DOWNLOAD
#define STDIN_MAX 100               //读取输入最大值
#define TRUE 1
#define FALSE 0
typedef struct linkMsg{
    int size;//总长度
    int flag;//标志位 读写浏览
    long fileSize;//8字节,文件大小
    char buf[512];//文件名，接收目录
}linkMsg_t;

/*客户端发给服务器的操作码*/
enum commend_num
{
    USERRNROLL,
    CDCOMMEND,
    LSCOMMEND,
    PUTSCOMMEND,
    GETSCOMMEND,
    RMCOMMEND,
    PWDCOMMEND
};

/*服务器发给客户端的确认码*/
enum flag_return
{
    FAIL_FLAG = 0,
    SUESSESS,
};
int aioWr(int toFd, char* buf, size_t length);
int tcp_client(char* ip, char* port);
int download(int socketFd);
int recvCycle(int sfd, void* buf, int fileSize);
int epollAdd(int epfd, int fd);
int getCommendFromStdin(char* dataStr);

