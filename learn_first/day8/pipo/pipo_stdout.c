#include <func.h>

int main(int argc, char* argv[])
{
    FILE* fp = popen("cat /etc/profile", "r");
    ERROR_CHECK(fp, NULL, "popen");
    char buf[512] = {0};
    while(fgets(buf, sizeof(buf)-1, fp)){
        puts(buf);
    }
    pclose(fp);
    return 0;
}

