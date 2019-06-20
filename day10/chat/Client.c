#include <func.h>
typedef struct mes{
    int client_id;
    char buf[512];
}MSG;

struct msgbuf{
    long mtype;
    char mtext[512];
};

int GLOBAL_QUIT = 0;
void handler(int sigNo, siginfo_t* pInfo, void *pReserve){
    GLOBAL_QUIT = 1;
}

int main(int argc, char* argv[])
{
    //注册信号
    struct sigaction sig;
    sig.sa_flags = SA_SIGINFO;
    sig.sa_sigaction = handler;
    sigaction(SIGINT, &sig, NULL);
    
    MSG myMsg;
    memset(&myMsg, 0, sizeof(myMsg));
    int client_fifo_fd, server_fifo_id;
    char client_fifo[256] = {0};
    int ret;
    struct msgbuf msgToWin;
    sprintf(client_fifo, "CLIENT_FIFO_%d", getpid());
    myMsg.client_id = getpid();
    server_fifo_id = open("./server", O_WRONLY);
    ERROR_CHECK(server_fifo_id, -1, "open");
    //server是写端，client是读端
    mkfifo(client_fifo, 0777);
    write(server_fifo_id, client_fifo, strlen(client_fifo));
    client_fifo_fd = open(client_fifo, O_RDONLY);
    ERROR_CHECK(client_fifo_fd, -1, "open");

    int msgid = msgget(1001, IPC_CREAT|0600);
    ERROR_CHECK(msgid, -1, "msgget");


    fd_set rdset;
    int fdReadyNum;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(client_fifo_fd, &rdset);
        fdReadyNum = select(client_fifo_fd+1, &rdset, NULL, NULL, NULL);
        if(1 == GLOBAL_QUIT){
            printf("Thanks! Goodbye.");
            break;
        }
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            ret = read(STDIN_FILENO, myMsg.buf, sizeof(myMsg.buf)-1);
            if(0 == ret){
                printf("close\n");
                break;
            }
            myMsg.buf[ret-1] = '\0';
            write(server_fifo_id, myMsg.buf, strlen(myMsg.buf));
            strcpy(msgToWin.mtext, myMsg.buf);
            msgToWin.mtype = 1;
            msgsnd(msgid, &msgToWin, strlen(msgToWin.mtext)+sizeof(long)+1,0);
        }
        if(FD_ISSET(client_fifo_fd, &rdset)){
            ret = read(client_fifo_fd, myMsg.buf, sizeof(myMsg.buf)-1);
            if(0 == ret){
                printf("byebye\n");
                break;
            }
            myMsg.buf[ret] = '\0';
            strcpy(msgToWin.mtext, myMsg.buf);
            msgToWin.mtype = 2;
            msgsnd(msgid, &msgToWin, strlen(msgToWin.mtext)+sizeof(long)+1, 0);
        }
    }
    close(client_fifo_fd);
    close(server_fifo_id);
    unlink(client_fifo);
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}

