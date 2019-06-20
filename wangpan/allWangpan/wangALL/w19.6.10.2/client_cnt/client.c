#include "client.h"

void printForCd(char* buf);
void printForPwd(char* buf);
void printForLs(char* buf);
void printForRm(char* buf);

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int ret,socketFd;
    socketFd = tcp_client(argv[1], argv[2]);
    ERROR_CHECK(socketFd, -1, "tcp_client");

    //先进行登录或者注册
    if(-1 == windowForLogin(socketFd)){
        printf("goodbye\n");
        return -1;
    }

    //监控输入端和服务器
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    struct epoll_event evs[2];
    epollAdd(epfd,socketFd);
    epollAdd(epfd, STDIN_FILENO);
    int readyNumOfFd;
    int i, commend;
    char bufTemp[STDIN_MAX];
    while(1){
        printf("请输入命令：(输入help查看帮助)\n");
        readyNumOfFd = epoll_wait(epfd, evs, 2, -1);
        for(i=0;i<readyNumOfFd;i++){
            if(evs[i].data.fd == socketFd){
                //服务器发信息，更新，活动
            }
            if(evs[i].data.fd == STDIN_FILENO){
                //这里思考一下
                commend = getCommendFromStdin(bufTemp);
                switch(commend){
                case CDCOMMEND:
                    simpleCommendDealFunc(socketFd, bufTemp, CDCOMMEND, printForCd);
                    break;
                case LSCOMMEND:
                    simpleCommendDealFunc(socketFd, bufTemp, LSCOMMEND, printForLs);
                    break;
                case PUTSCOMMEND:
                    //puts
                    ret = upload(socketFd, bufTemp);
                    break;
                case GETSCOMMEND:
                    //gets  需要传送文件名，结构体
                    ret = download(socketFd, bufTemp);
                    if(-1 == ret){
                        goto END;
                    }
                    break;
                case RMCOMMEND:
                    simpleCommendDealFunc(socketFd, bufTemp, RMCOMMEND, printForRm);
                    break;
                case PWDCOMMEND:
                    simpleCommendDealFunc(socketFd, bufTemp,PWDCOMMEND, printForPwd);
                    break;
                case HELPCOMMEND:
                    helpManual();
                    break;
                default:
                    break;
                }

            }
        }
    }
END:
    close(socketFd);
    return 0;
}

int epollAdd(int epfd, int fd)
{  
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    int ret;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    return 0;
}

void printForCd(char* buf)
{
    printf("%s\n", buf);
}

void printForPwd(char* buf)
{
    printf("当前路径为：%s\n", buf);
}

void printForLs(char* buf)
{   
    long fileSize;
    char fileType;
    char fileName[255];
    int offset = 0;
    while(sscanf(buf + offset, "%c %ld %s", &fileType, &fileSize, fileName) != EOF)
    {
        printf("%2c %-15ld %-30s\n", fileType, fileSize, fileName);
        offset += 12+strlen(fileName);
    }
}

void printForRm(char* buf)
{
    printf("%s\n", buf);
}
