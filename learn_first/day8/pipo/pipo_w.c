#include <func.h>

int main(int argc, char* argv[])
{
    char buf[] = {"aaa bbb ccc ddd eee fff"};
    FILE* fp = popen("wc -w", "w");
    fwrite(buf, sizeof(buf), 1,fp);
    pclose(fp);
    return 0;
}

