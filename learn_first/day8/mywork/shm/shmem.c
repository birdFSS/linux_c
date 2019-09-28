#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    key_t key = ftok(argv[1], 1);
    printf("key = %d\n", key);
    int shmid = shmget(key, 4096, 0600| IPC_CREAT);
    if(shmid == -1){
        perror("shmget");
    }
    if(fork() > 0){
        char* p_addr = (char*)shmat(shmid, NULL, 0);
        memset(p_addr,0, 4096);
        strcpy(p_addr, "How are you");
        printf("write buf = %s\n", p_addr);
        sleep(2);
        wait(NULL);
        shmctl(shmid, IPC_RMID, 0);
        exit(0);
    }else{
        sleep(3);
        char *p_addr = (char*)shmat(shmid, NULL, 0);
        printf("child read = %s\n", p_addr);
        exit(0);
    }
    return 0;
}

