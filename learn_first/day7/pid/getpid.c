#include <func.h>

int main(int argc, char* argv[])
{
    printf("pid = %d ppid = %d", getpid(), getppid());
    return 0;
}

