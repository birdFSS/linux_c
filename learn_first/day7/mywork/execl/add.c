#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    printf("sum of a and b is %d\n", a+b);
    return 0;
}

