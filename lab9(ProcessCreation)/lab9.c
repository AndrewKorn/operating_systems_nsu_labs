#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main() {
    pid_t pid, ret;
    if ((pid = fork()) == 0) {
        execlp("cat", "cat", "text.txt", (char*)0);
        perror("exec failed");
        exit(1);
    }

    ret = wait(NULL);
    printf("\n%d(parent) printf after waiting %d(child)\n", getpid(), ret);
    return 0;
}

