#include <stdio.h>
#include <glob.h>


int main(int argc, char* argv[]) {
    glob_t glob_buf;

    if (argc < 2) {
        printf("No enough args\n");
        return 0;
    }
    
    if ((glob(argv[1], 0, NULL, &glob_buf)) == 0) {
        if (glob_buf.gl_pathc == 0) {
            printf("There is no matches");
        }
        else {
            for (int i = 0; i < glob_buf.gl_pathc; ++i) {
                printf("%s\n", glob_buf.gl_pathv[i]);
            }
        }
        globfree(&glob_buf);
    }
    else {
        perror("glob");
    }
    return 0;
}

