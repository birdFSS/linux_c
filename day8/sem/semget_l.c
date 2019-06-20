#include <func.h>

int main(int argc, char* argv[])
{
    int ret = semctl(0, 0, IPC_RMID, 1);
    ERROR_CHECK(ret, -1, "semctl");
    return 0;
}

