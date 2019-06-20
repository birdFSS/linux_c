#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc , 2);
    DIR* dir = opendir(argv[1]);
    ERROR_CHECK(dir, NULL, "opendir");
    struct dirent *d;
    while((d=readdir(dir))!=NULL){
        printf("%d %ld %s\n", d->d_reclen, d->d_ino, d->d_name);
    }
    return 0;
}

