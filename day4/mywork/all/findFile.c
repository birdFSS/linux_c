#include <func.h>

int findFile(char* path,char* fileName){
    DIR* dir;
    dir=opendir(path);
    ERROR_CHECK(dir,NULL,"opendir");
    struct dirent *p;
    char tmpPath[1024]={0};
    while((p=readdir(dir))!=NULL){
        if(!strcmp(p->d_name, ".")|| !strcmp(p->d_name,"..")){
            continue;
        }
        if(4 == p->d_type){
            sprintf(tmpPath,"%s%s%s", path, "/", p->d_name);
            findFile(tmpPath,fileName);
        }
        if(!strcmp(p->d_name,fileName)){
            printf("%s%s%s\n",path,"/",p->d_name);
        }
    }
    closedir(dir);
    return 0;
}

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc, 3);
    findFile(argv[1],argv[2]);
    return 0;
}

