#include <func.h>

int main(int argc, char* argv[])
{
    int ret;
    ret = send(8, "hello", 5, 0);
    //ERROR_CHECK(ret, -1, "send");
    printf("return is %d\n", ret);
    return 0;
}

