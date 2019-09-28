#include <func.h>

#define BUFFER_SIZE 1024

void aioHandler(int signo, siginfo_t* info, void* content){
    //这时异步操作结束，释放资源，回收文件描述符
    struct aiocb *req;
    int ret;
    if(info->si_signo == SIGIO){
        req = (struct aiocb *)info->si_value.sival_ptr;
        if(aio_error(req) == 0){
            printf("传输完成\n");
            ret = aio_return(req);
            write(STDOUT_FILENO, (void*)req->aio_buf, req->aio_nbytes);
        }
        close(req->aio_fildes);
        free((void*)req->aio_buf);
    }
}


int main(int argc,char **argv)
{
    struct aiocb rd;

    int fd,ret;

    fd = open("aio_write.i",O_RDONLY);
    if(fd < 0)
    {
        perror("test.txt");
    }
    //将rd结构体清空
    bzero(&rd,sizeof(rd));
    //为rd.aio_buf分配空间
    rd.aio_buf = malloc(BUFFER_SIZE + 1);
    bzero((void*)rd.aio_buf, BUFFER_SIZE+1);
    //填充rd结构体
    rd.aio_fildes = fd;
    rd.aio_nbytes =  BUFFER_SIZE;
    rd.aio_offset = 0;
    //通知方式设定,函数结束栈空间会释放，堆空间不会
    rd.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    //将rd的地址传进去,也可以传入你想要的结构体指针（堆）
    rd.aio_sigevent.sigev_value.sival_ptr = &rd; 
    rd.aio_sigevent.sigev_signo = SIGIO;
    
    //注册信号
    struct sigaction sig_act;
    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = SA_SIGINFO;
    sig_act.sa_sigaction = aioHandler;
    ret = sigaction(SIGIO, &sig_act, NULL);
    //进行异步读操作
    ret = aio_read(&rd);
    if(ret < 0)
    {
        perror("aio_read");
        exit(1);
    }
    sleep(2);//防止进程终止，等一下
    //获取异步读返回值
    ret = aio_return(&rd);//这个不会阻塞，直接返回0

    printf("\n\n返回值为:%d\n",ret);    

    //printf("%s\n", (char*)rd.aio_buf);//不等待异步返回，就会导致直接输出 空字符串
    return 0;
}





