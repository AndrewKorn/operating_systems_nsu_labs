#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void cancel_handler(void* args) {
    printf("Thread cancelled\n");
}

void* new_thread_func(void* args) {
    pthread_cleanup_push(cancel_handler, NULL);
    while(1) {
        printf("thread work\n");
    }
    pthread_cleanup_pop(0);
}

int main() {
    pthread_t tid;

    int status;
    if ((status = pthread_create(&tid, NULL, new_thread_func, NULL)) != 0) {
        fprintf(stderr, "can't create thread, status = %d\n", status);
        return 1;
    }

    sleep(2);

    if ((status = pthread_cancel(tid)) != 0) {
        fprintf(stderr, "can`t cancel thread, status = %d\n", status);
    }

    printf("Hello, World!\n");
    return 0;
}
