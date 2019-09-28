#include <func.h>
struct text{
    int useNum;//useNum 1 本地  useNum 2 对方
    char buf[512];
};
int main(int argc, char* argv[])
{
    int shmid = shmget(1000, 4096, IPC_CREAT|0600);
    struct text *pText =(struct text*)shmat(shmid, NULL, 0);
    ERROR_CHECK(pText, (struct text *)-1, "shmat");
    while(1){
        if(pText->useNum == 1){//本地发言，在右边
            printf("%40s%s\n", " ", pText->buf);
            pText->useNum = 0;
        }
        else if(pText->useNum == 2){
            printf("%s\n",pText->buf);
            pText->useNum = 0;
        }
        else if(pText->useNum == 6){
            break;
        }
    }
    return 0;
}

