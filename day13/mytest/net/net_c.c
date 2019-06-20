#include <func.h>

int main(int argc, char* argv[])
{
    short port = 0x1234;
    short nport;
    nport = htons(port);
    printf("nport=%x\n", nport);
    return 0;
}

