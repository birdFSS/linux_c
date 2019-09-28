#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    //由文件的绝对路径，返回一个关键字。
    key_t key = ftok(argv[1], 1);
    printf("%d\n", key);
    return 0;
}

