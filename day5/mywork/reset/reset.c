#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int fPrint = open(argv[1], O_RDWR| O_TRUNC);
    ERROR_CHECK(fPrint, -1, "open");
    int fPerror = open(argv[2], O_RDWR| O_TRUNC);
    ERROR_CHECK(fPerror, -1, "open");
    int fpr2 = dup2(fPrint,STDOUT_FILENO);
    int fpe2 = dup2(fPerror, STDERR_FILENO);
    printf("hello I am printf");
    perror("hello I am perror");
    return 0;
}

