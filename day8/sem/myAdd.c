#include <func.h>
#define N 10000000
int main(int argc, char* argv[])
{
    int shmid = shmget(1000, 4096, 0600| IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    int *p = (int *)shmat(shmid, NULL, 0);
    ERROR_CHECK(p, (int *)-1, "shmat");
    memset(p, 0, 4096);
    int semId = semget(1000, 1, 0600|IPC_CREAT);
    ERROR_CHECK(semId, -1, "semget");
    int res = semctl(semId, 0, SETVAL, 1);
    ERROR_CHECK(res, -1, "semctl");
    struct sembuf sp,sv;
    sp.sem_num = 0;
    sp.sem_op = -1;
    sp.sem_flg = SEM_UNDO;

    sv.sem_num =  0;
    sv.sem_op = 1;
    sv.sem_flg = SEM_UNDO;

    if(!fork()){
        //child
        for(int i=0;i<N;i++){
            semop(semId, &sp, 1);
            *p+=1;
            semop(semId, &sv, 1);
        }
        exit(0);
    }else{
        //parent
        for(int i=0;i<N;i++){
            semop(semId, &sp, 1);
            *p+=1;
            semop(semId, &sv, 1);
        }
        wait(NULL);
        printf("res = %d\n", *p);
        shmctl(shmid, IPC_RMID, 0);
        semctl(semId, 0, IPC_RMID);
        exit(0);
    
    }

}

