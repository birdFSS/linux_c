#include <func.h>

typedef struct mes{
   int server_pid;
   char buf[512];
}MSG;

struct text{
    int useNum;
    char buf[512];
};

int main(int argc, char* argv[])
{
    MSG myMsg;
    memset(&myMsg, 0, sizeof(myMsg));
    //共享内存，窗口
    int shmid = shmget(1000, 4096, IPC_CREAT|0600);
    struct text *shared = (struct text*)shmat(shmid, NULL, 0);
    ERROR_CHECK(shared, (struct text*)-1, "shmat");
    
    mkfifo("server", 0666);
    //server是读端
    int server_fifo_fd = open("./server", O_RDONLY);
    int ret = read(server_fifo_fd, myMsg.buf, sizeof(myMsg.buf));
    ERROR_CHECK(ret, -1, "read");
    //client是写端
    int client_fifo_fd = open(myMsg.buf, O_WRONLY);
    ERROR_CHECK(client_fifo_fd, -1, "open");

    ret = read(server_fifo_fd, myMsg.buf, sizeof(myMsg.buf));
    myMsg.buf[ret] = '\0';
    printf("%s\n", myMsg.buf);
    
    //监控
    fd_set rdset;
    int fdReadyNum;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(server_fifo_fd, &rdset);
        fdReadyNum = select(server_fifo_fd+1, &rdset, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            ret = read(STDIN_FILENO, myMsg.buf, sizeof(myMsg.buf)-1);
            if(0 == ret){
                printf("close\n");
                break;
            }
            myMsg.buf[ret-1] = '\0';
            write(client_fifo_fd, myMsg.buf, strlen(myMsg.buf));
            shared->useNum = 1;
            strcpy(shared->buf, myMsg.buf);
        }
        if(FD_ISSET(server_fifo_fd, &rdset)){
            ret = read(server_fifo_fd, myMsg.buf, sizeof(myMsg.buf)-1);
            if(0 == ret){
                printf("byebye\n");
                break;
            }
            myMsg.buf[ret-1] = '\0';
            shared->useNum = 2;
            strcpy(shared->buf, myMsg.buf);
        }
    }
    shared->useNum = 6;
    shmdt(shared);
    shmctl(shmid, IPC_RMID, 0);
    return 0;
}

