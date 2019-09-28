#include <func.h>
//改变全局变量后，观察栈空间或堆空间是否相同，确定复制是写时全部复制，还是写时部分复制。
int global = 10;

int main(int argc, char* argv[])
{
    printf("Parent process id = %d\n", getpid());
    int iStack =100;
    pid_t ret = fork();
    if(ret < 0){
        printf("Create error");
    }else if(0 == ret){
        printf("global in child = %d global p = %p iStakc = %d\n", global,&global ,iStack);
        printf("I am child, pid = %d, myparent id = %d\n", getpid(), getppid());
    }else{
        global = 5;
        int *p = &iStack;
        *p = 50;
        printf("global in parent = %d global p = %p iStack = %d\n", global,&global,iStack);
        printf("I am parent,pid = %d myparent id = %d\n", getpid(), getppid());
        sleep(1);
    }
    return 0;
}

