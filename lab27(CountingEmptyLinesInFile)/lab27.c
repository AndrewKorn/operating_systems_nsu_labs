#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    FILE* fpin;
    FILE* fpout;
    char buf[BUFSIZ];

    if ((fpin = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);
        return -1;
    }

    fpout = popen("wc -l", "w");

    int cary = 0;
    while (fgets(buf, BUFSIZ, fpin) != NULL) {
        int len = strlen(buf);
        if (len < BUFSIZ - 1) {
            if (buf[0] == '\n') {
                if (cary == 0) {
                    fputc('\n', fpout);
                }
                else {
                    cary = 0;
                }
            }
        }
        else {
            cary = 1;
        }
    }

    fclose(fpin);
    pclose(fpout);
    return 0;
}

