#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <libgen.h>

int main(int argc, char* argv[]) {
    struct stat stbuf;

    for (int i = 1; i < argc; ++i) {
        if (stat(argv[i], &stbuf) == -1) {
            perror(argv[0]);
            exit(1);
        }

        char type;
        switch (stbuf.st_mode & S_IFMT) {
            case S_IFDIR:
                type = 'd';
                break;
            case S_IFREG:
                type = '-';
                break;
            default:
                type = '?';
        }
        printf("%c", type);

        (stbuf.st_mode & S_IRUSR) ? printf("r") : printf("-");
        (stbuf.st_mode & S_IWUSR) ? printf("w") : printf("-");
        (stbuf.st_mode & S_IXUSR) ? printf("x") : printf("-");

        (stbuf.st_mode & S_IRGRP) ? printf("r") : printf("-");
        (stbuf.st_mode & S_IWGRP) ? printf("w") : printf("-");
        (stbuf.st_mode & S_IXGRP) ? printf("x") : printf("-");

        (stbuf.st_mode & S_IROTH) ? printf("r") : printf("-");
        (stbuf.st_mode & S_IWOTH) ? printf("w") : printf("-");
        (stbuf.st_mode & S_IXOTH) ? printf("x") : printf("-");

        printf(" %3d", stbuf.st_nlink);

        struct passwd* pw = getpwuid(stbuf.st_uid);
        if (pw != NULL) {
            printf(" %8s", pw->pw_name);
        }
        else {
            printf(" %8d", stbuf.st_uid);
        }

        struct group* grp = getgrgid(stbuf.st_gid);
        if (grp != NULL) {
            printf(" %8s", grp->gr_name);
        }
        else {
            printf(" %8d", stbuf.st_gid);
        }

        if (type == '-') {
            printf(" %8d ", stbuf.st_size);
        }
        else {
            printf("          ");
        }

        char* mtime = ctime(&stbuf.st_mtime);
        for (int j = 4; j < strlen(mtime) - 9; ++j) {
            printf("%c", mtime[j]);
        }
        
        printf(" %s\n", basename(argv[i]));
    }
    return 0;
}
