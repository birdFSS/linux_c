#include "client.h"

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int ret,socketFd;
    socketFd = tcp_client(argv[1], argv[2]);
    if(-1 == socketFd){
        return -1;
    }
    ret = download(socketFd);
    if(-1 == ret){
        goto END;
    }
END:
    close(socketFd);
    return 0;
}
