#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* new_thread_work(void* args) {
    for (int i = 0; i < 10; ++i) {
        printf("%d Hello from another thread\n", i);
    }
}

int main() {
    pthread_t tid;

    int status;
    if ((status = pthread_create(&tid, NULL, new_thread_work, NULL)) != 0) {
        fprintf(stderr, "can't create thread, status =  %d\n", status);
        return 1;
    }

    if ((status = pthread_join(tid, NULL)) != 0) {
        fprintf(stderr, "can't join, status =  %d\n", status);
        return 1;
    }

    for (int i = 0; i < 10; ++i) {
        printf("%d Hello from main thread\n", i);
    }
    return 0;
}
