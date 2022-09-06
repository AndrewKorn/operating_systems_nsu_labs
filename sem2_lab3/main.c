#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* new_thread_work(void* args) {
    printf("%s\n", (char*)args);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "invalid number of arguments, needs 4\n");
        return 1;
    }

    pthread_t tid[4];

    int status;
    for (int i = 0; i < 4; ++i) {
        if ((status = pthread_create(&tid[i], NULL, new_thread_work, argv[i + 1]) )!= 0) {
            fprintf(stderr, "can't create thread %d, status =  %d\n", i, status);
            return 1;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if ((status = pthread_join(tid[i], NULL)) != 0) {
            fprintf(stderr, "can't join %d, status = %d\n", i, status);
            return 1;
        }
    }
    return 0;
}
