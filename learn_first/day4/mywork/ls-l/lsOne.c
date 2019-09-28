#include <func.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc,2);
    DIR* dir;
    dir = opendir(argv[1]);
    ERROR_CHECK(dir,NULL,"opendir");
    struct dirent *p;
    struct stat buf;
    int ret;
    char path[1024];
    char date1[50];
    while((p=readdir(dir))!=NULL){
        memset(path,0,sizeof(path));

        sprintf(path,"%s%s%s",argv[1],"/",p->d_name);
        ret = stat(path,&buf);
        ERROR_CHECK(ret, -1,"stat");
        strncpy(date1,ctime(&buf.st_mtime),16);
        date1[16]='\0';
        memmove(date1,date1+4,strlen(date1)+1);
        printf(" %x %ld %s %s %8ld %12s %s\n",buf.st_mode, buf.st_nlink,\
            getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,\
            buf.st_size, date1,p->d_name);
    }
    closedir(dir);
    return 0;
}

