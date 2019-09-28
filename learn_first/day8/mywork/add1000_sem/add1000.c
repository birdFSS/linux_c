#include <func.h>
#define N 10000000
int main(int argc, char* argv[])
{
    int shmid = shmget(1000, 4096, 0600| IPC_CREAT);
    if(!fork()){
        //child
        int cnt= 0;
        long* p_child = (long *)shmat(shmid, NULL, 0);  
        for(int i=0;i<N;i++){
            p_child[0]+=1;
            ++cnt;
        }
        printf("parent Num = %d\n", cnt);
        exit(0);
    }else{
        int cnt = 0;
        long* p_child = (long *)shmat(shmid, NULL, 0);  
        for(int i=0;i<N;i++){
            ++cnt;
            p_child[0]+=1;
        }
        printf("parent Num = %d\n", cnt);
        wait(NULL);
        printf("%ld\n", p_child[0]);
        shmctl(shmid, IPC_RMID, 0);
        exit(0);
    }
    return 0;
}

