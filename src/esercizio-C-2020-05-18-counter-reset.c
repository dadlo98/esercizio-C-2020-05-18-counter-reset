#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>       
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

#define LIMIT 100000
#define RESET_LIMIT 100

int counter;
int reset;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * thread_function(void * arg);

int main(void) {
    pthread_t t1, t2;
    int res;
    void * s;

    res = pthread_create(&t1, NULL, thread_function, NULL);
    if (res != 0) {
        perror("pthread_create");
    }

    res = pthread_create(&t2, NULL, thread_function, NULL);
    if (res != 0) {
        perror("pthread_create");
    }

    res = pthread_join(t1, &s);
    if( res != 0) {
        perror("pthread_join");
    }

    res = pthread_join(t2, &s);
    if( res != 0) {
        perror("pthread_join");
    }
    /*if(reset > 100){
        printf("\nreset exceeded\n");
    }*/
    else{
        printf("\ncounter  = %d , reset = %d\n", counter, reset);
    }
    return 0;
}

void * thread_function(void * args) {
    int s;
    while(1) {
        s = pthread_mutex_lock(&mutex);
        if(s != 0) {
            perror("lock");
            exit(EXIT_FAILURE);
        }
        counter++;
        if (counter > LIMIT) {
            counter = counter - LIMIT;
            reset++;
        }
        s = pthread_mutex_unlock(&mutex);
        if(s != 0) {
            perror("lock");
            exit(EXIT_FAILURE);
        }
        if (reset > RESET_LIMIT)
            return NULL;
    }
}
