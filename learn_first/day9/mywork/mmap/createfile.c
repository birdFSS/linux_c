#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    char c = 'a';
    FILE *fp = fopen("file", "w");
    for(int i=0;i<4097;i++){
       fprintf(fp,"%c", c);
    }
    return 0;
}

