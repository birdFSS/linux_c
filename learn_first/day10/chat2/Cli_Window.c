#include <func.h>
struct msgbuf{
    long mtype;
    char mtext[512];
};
int main(int argc, char* argv[])
{
    system("clear");
    struct msgbuf msg;
    int msgid = msgget(1001, IPC_CREAT|0600);
    while(1){
        ssize_t len = msgrcv(msgid, &msg, sizeof(msg), 0, 0);
        ERROR_CHECK(len, -1, "msgrcv");
        //1 本地 2对方
        if(msg.mtype == 1){
            printf("%40s%s\n", " ",msg.mtext);
        }else if(msg.mtype == 2){
            printf("%s\n", msg.mtext);
        }
    }   

    return 0;
}

