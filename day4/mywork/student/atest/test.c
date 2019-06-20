#include <func.h>

typedef struct student{
    int id;
    char name[30];
    float score;
}stu, pstu;

void print_stu(stu* s, int len){
    for(int i=0;i<len;i++){
        printf("%6d%20s%6.2f\n", s[i].id,s[i].name,s[i].score);
    } 
}

int main(int argc, char* argv[])
{
    stu s[3] = {{10001,"xiongda",60.5},{10002,"xionger",90.1},{10003,"xiongsan",99.5}};
    int fd=open("student.txt",O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    char buf[100];
    memset(buf, 0, 100);
    for(int i=0;i<3;i++){  
        sprintf(buf+strlen(buf),"%d %s %f\n", s[i].id,s[i].name,s[i].score);
    }
    printf("fd1=%d\n", fd);
    print_stu(s,3);
    int len = write(fd,buf,strlen(buf));
    printf("fd2=%d\n", fd);
    lseek(fd,0,SEEK_SET);
    memset(s,0,sizeof(stu)*3);
    int readNum =  read(fd, buf, len);
    printf("after read fd=%d", fd);
    printf("readNum=%d\n", readNum);
    printf("len = %d\n", len);
    print_stu(s,3);
    for(int i=0;i<3;i++){
        if(i==0){
            sscanf(buf, "%d%s%f", &s[i].id, s[i].name, &s[i].score);
        }else{
            sscanf(buf+10+strlen(s[i-1].name), "%d%s%f", &s[i].id, s[i].name, &s[i].score);
        }
    }
    print_stu(s,3);
    return 0;
}

