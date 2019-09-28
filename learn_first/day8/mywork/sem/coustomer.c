#include <func.h>

int main()
{
    int semId = semget(1000, 2, IPC_CREAT|0600);
    ERROR_CHECK(semId, -1, "semget");
    unsigned short arr[2] = {10, 0};//第一个信号表示苹果数，第二个信号表示盘子数
    int res = semctl(semId, 0, SETALL , arr);//将所有参数设置进去，第二个参数为0
    ERROR_CHECK(res, -1, "semctl");
    struct sembuf sp, sv;
    if(!fork()){
        //子进程吃苹果
        sp.sem_num = 0;
        sp.sem_op = -1;
        sp.sem_flg = SEM_UNDO;
        sv.sem_num = 1;
        sv.sem_op = 1;
        sv.sem_flg = SEM_UNDO;
        while(1){
            printf("child apples = %d, plates = %d\n", semctl(semId, 0, GETVAL), semctl(semId, 1,GETVAL));
            semop(semId, &sp, 1);
            printf("child is eating the apple\n");
            semop(semId, &sv, 1);
            sleep(1);
        }
        exit(0);
    }else{
        //父进程削苹果
        sp.sem_num = 1;
        sp.sem_op = -1;
        sp.sem_flg = SEM_UNDO;
        sv.sem_num = 0;
        sv.sem_op = 1;
        sv.sem_flg = SEM_UNDO;
        while(1){
            printf("parent apples = %d, plates = %d\n", semctl(semId, 0, GETVAL), semctl(semId, 1,GETVAL));
            semop(semId, &sp, 1);
            printf("parent is peeling apple\n");
            semop(semId, &sv, 1);
            sleep(2);
        }
        wait(NULL);
    }

    return 0;
}

