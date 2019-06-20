#include <func.h>

int main(int argc, char* argv[])
{
    printf("page size = %ld\n", sysconf(_SC_PAGESIZE));
    printf("uid = %d gid = %d euid = %d egid = %d\n", getuid(), getgid(), geteuid(), getegid());
    return 0;
}

