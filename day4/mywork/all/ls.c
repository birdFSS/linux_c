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
    int i,cnt;
    while((p=readdir(dir))!=NULL){
        memset(path,0,sizeof(path));
        i = 1 << 15;
        cnt = 1;
        sprintf(path,"%s%s%s",argv[1],"/",p->d_name);
        ret = stat(path,&buf);
        ERROR_CHECK(ret, -1,"stat");
        strncpy(date1,ctime(&buf.st_mtime),16);
        date1[16]='\0';
        memmove(date1,date1+4,strlen(date1)+1);
        unsigned short mode = buf.st_mode;
        while(i != 0){
            if((mode & i) != 0){
                break;
            }
            cnt++;
            i = i >> 1;
        }
        switch(cnt){
        case 1:   printf("-"); break;
        case 2:   printf("d"); break;
        case 3:   printf("d"); break;//这里不知道后面的对应什么文件
        case 4:   printf("d"); break;
        case 5:   printf("d"); break;
        case 6:   printf("d"); break;
        case 7:   printf("d"); break;
        }
        i = 1 << 8;
        while(i!=0){
            if((i&mode)!=0){
                printf("r");
            }else{
                printf("-");
            }
            i = i >> 1;
            if((i&mode)!=0){
                printf("w");
            }else{
                printf("-");
            }
            i = i >> 1;
            if((i&mode)!=0){
                printf("x");
            }else{
                printf("-");
            }
            i = i>>1;
        }
        printf(" %ld %s %s %8ld %12s %s\n", buf.st_nlink,\
            getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,\
            buf.st_size, date1,p->d_name);
    }
    closedir(dir);
    return 0;
}

