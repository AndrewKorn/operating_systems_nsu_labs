#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdbool.h>

typedef struct LineInfo {
    off_t offset;
    off_t length;
} LineInfo;

int main(int argc, char* argv[]) {
    int fd;
    char* pa;

    if (argc < 2) {
        perror("invalid number of arguments");
        return 1;
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        return 1;
    }

    struct stat sbuf;
    if (fstat(fd, &sbuf) == -1) {
        perror("stat");
        return 1;
    }

    if ((pa = mmap(0, sbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == (char*)-1 ) {
        perror("mmap");
        return 1;
    }


    int MAX_LINE_COUNT = 256;
    LineInfo* table = malloc(MAX_LINE_COUNT * sizeof(LineInfo));
    if (table == NULL) {
        perror("table");
        return 1;
    }

    table[0].offset = 0;
    table[0].length = 0;
    int count = 0;
    for (off_t i = 0; i < sbuf.st_size; ++i) {
        if (*(pa + i) == '\n') {
            table[count].length = i - table[count].offset;
            ++count;
            table[count].offset = i + 1;
        }

        if (count == MAX_LINE_COUNT - 1) {
            MAX_LINE_COUNT *= 2;
            table = realloc(table, MAX_LINE_COUNT * sizeof(LineInfo));
            if (table == NULL) {
                perror("realloc");
            }
        }
    }


    fd_set fdset;
    setbuf(stdin, NULL);
    while (true) {
        char buf[BUFSIZ] = {0};
        printf("You have 5 seconds to enter a line number (0 - to exit)\n");

        FD_ZERO(&fdset);
        FD_SET(STDIN_FILENO, &fdset);
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        int result = select(1, &fdset, NULL, NULL, &timeout);

        if (result == -1) {
            perror("select");
            return 1;
        }
        if (result == 0) {
            printf("Time is up, print all lines\n");
            write(1, pa, sbuf.st_size);
            break;
        }
        else {
            int lineNumber;
            if (scanf("%d", &lineNumber) != 1) {
                fflush(stdin);
                perror("Invalid input\n");
                continue;
            }

            if (lineNumber < 0 || lineNumber > count) {
                printf("bad line number\n");
                continue;
            }
            if (lineNumber != 0) {
                write(1, pa + table[lineNumber - 1].offset, table[lineNumber - 1].length + 1);
            }
            else {
                break;
            }
        }
    }

    close(fd);
    munmap(pa, sbuf.st_size);
    return 0;
}

