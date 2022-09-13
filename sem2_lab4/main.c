#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* new_thread_func(void* args) {
    while(1) {
        printf("thread work\n");
    }
}

int main() {
    pthread_t tid;

    int status;
    if ((status = pthread_create(&tid, NULL, new_thread_func, NULL)) != 0) {
        fprintf(stderr, "can't create thread, status = %s\n", strerror(status));
        return 1;
    }

    sleep(2);

    if ((status = pthread_cancel(tid)) != 0) {
        fprintf(stderr, "can`t cancel thread, status = %s\n", strerror(status));
    }

    printf("Hello, World!\n");
    return 0;
}
