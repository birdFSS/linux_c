#include <func.h>
int glo_quit = 0;

int client_init(char* ip, char* port){
    int socketFd;
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serAddr;
    bzero(&serAddr, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);
    int ret;
    ret = connect(socketFd, (struct sockaddr*)&serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "connect");
    return socketFd;
}


int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int socketFd = client_init(argv[1], argv[2]);
    fd_set rdset;
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    char buf[512] = {0};
    int ret = 0;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(socketFd, &rdset);
        ret = select(socketFd+1, &rdset, NULL, NULL, &tv);
        if(ret == -1){
            perror("select");
            return -1;
        }else if(glo_quit == 1){
            printf("goodbye!Thanks for using\n");
            break;
        }
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            ret = read(STDIN_FILENO, buf, sizeof(buf)-1);
            if(0 == ret){
                break;
            }
            buf[ret - 1] = '\0';
            send(socketFd, buf, strlen(buf), 0);
        }

        if(FD_ISSET(socketFd, &rdset)){
            memset(buf, 0, sizeof(buf));
            ret = recv(socketFd, buf, sizeof(buf), 0);
            if(-1 == ret){
                perror("recv");
                close(socketFd);
                close(socketFd);
            }else if(0 == ret){
                break;
            }
            printf("%s\n", buf);
        }
    }
    close(socketFd);
    return 0;
}

