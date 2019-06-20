#include <func.h>

int main(int argc, char* argv[])
{
    printf("pid = %d ppid = %d\ntrue user id = %d true group id = %d\nreal user id = %d real group id = %d\n", \
          getpid(), getppid(), getuid(), getgid(), geteuid(), getegid());
    return 0;
}

