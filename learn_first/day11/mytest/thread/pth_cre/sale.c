#include <func.h>

typedef struct thread_msg{
    int tickets;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}thrMsg;
void* sale1(void* p){
    thrMsg* msg = (thrMsg*)p;
    while(1){
        pthread_mutex_lock(&msg->mutex);
        if(msg->tickets > 0){
            printf("I am sale1, saling a ticket, tickets = %d\n", msg->tickets);
            msg->tickets--;
            if(!msg->tickets){
                pthread_cond_signal(&msg->cond);
            }
            printf("after sale tickets = %d\n", msg->tickets);
            pthread_mutex_unlock(&msg->mutex);
            usleep(1000);
        }else{
            pthread_mutex_unlock(&msg->mutex);
            break;
        }

    }
    pthread_exit(NULL);
}

void* sale2(void* p){
    thrMsg* msg = (thrMsg*)p;
    while(1){
        pthread_mutex_lock(&msg->mutex);
        if(msg->tickets > 0){
            printf("I am sale2, saling a ticket, tickets = %d\n", msg->tickets);
            msg->tickets--;
            if(!msg->tickets){
                pthread_cond_signal(&msg->cond);
            }
            printf("after sale tickets = %d\n", msg->tickets);
            pthread_mutex_unlock(&msg->mutex);
            usleep(1000);
        }else{
            pthread_mutex_unlock(&msg->mutex);
            break;
        }
    }
    pthread_exit(NULL);
}

void* putTicket(void* p){
    thrMsg* msg = (thrMsg*)p;
    pthread_mutex_lock(&msg->mutex);
    if(msg->tickets >0)
        pthread_cond_wait(&msg->cond, &msg->mutex);
    msg->tickets = 20;
    pthread_mutex_unlock(&msg->mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t pthid1, pthid2, pthid3;
    thrMsg msg;
    msg.tickets = 20;
    pthread_mutex_init(&msg.mutex, NULL);
    pthread_cond_init(&msg.cond, NULL);
    pthread_create(&pthid1, NULL, sale1, &msg);
    pthread_create(&pthid2, NULL, sale2, &msg);
    pthread_create(&pthid3, NULL, putTicket, &msg);
    pthread_join(pthid1, NULL);
    pthread_join(pthid2, NULL);
    pthread_join(pthid3, NULL);
    printf("I am main tickets = %d", msg.tickets);
    return 0;
}

