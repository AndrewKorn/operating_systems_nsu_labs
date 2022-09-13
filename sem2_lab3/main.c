#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* new_thread_work(void* args) {
    printf("%s\n", (char*)args);
}

int main() {
    char* lines[4] = {"First line", "Second line", "Third line", "Fourth line"};
    pthread_t tid[4];

    int status;
    for (int i = 0; i < 4; ++i) {
        if ((status = pthread_create(&tid[i], NULL, new_thread_work, lines[i]) )!= 0) {
            fprintf(stderr, "can't create thread %d, status =  %s\n", i, strerror(status));
            return 1;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if ((status = pthread_join(tid[i], NULL)) != 0) {
            fprintf(stderr, "can't join %d, status = %s\n", i, strerror(status));
            return 1;
        }
    }
    return 0;
}
