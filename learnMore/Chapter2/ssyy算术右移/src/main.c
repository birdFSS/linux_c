#include <func.h>

int main(int argc, char* argv[])
{
    int i = 0x80000001;
    printf("0x%x\n", i);
    printf("%d\n", i);
    i = i >> 1;
    printf("0x%x\n", i);
    printf("%d\n", i);

    int a = -1;
    printf("%x\n", a);
    return 0;
}

