#include <func.h>
#define N 10000000
int main(int argc, char* argv[])
{
    int shmid;
    shmid = shmget(1000, 4096, 0600| IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    printf("shmid = %d\n", shmid);
    int *p = (int *)shmat(shmid, NULL, 0);
    printf("%d\n", *p);
    int i;
    if(!fork()){
        for(i=0;i<N;i++){
            *p+=1;
        }
        return 0;
    }else{
        for(i=0;i<N;i++){
            *p+=1;
        }
        wait(NULL);
        printf("result = %d\n", *p);
        return 0;
    }
}

