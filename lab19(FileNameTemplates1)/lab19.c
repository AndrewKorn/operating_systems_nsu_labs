#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

bool isSuit(char* fname, char* template) {
    char *backFname = 0, *backTemplate;
    while (true)
        if (*template == '*') {
            backFname = fname;
            backTemplate = ++template;
        }
        else if(!*fname) {
            return !*template;
        }
        else if (*fname == *template || *template == '?') {
            ++fname;
            ++template;
        }
        else if (backFname) {
            fname = ++backFname;
            template = backTemplate;
        }
        else {
            return false;
        }
}

int main(int argc, char* argv[]) {
    DIR *dirp;
    struct dirent *dp;

    if ((dirp = opendir(".")) == NULL) {
        perror("opendir");
        return -1;
    }

    while ((dp = readdir(dirp)) != NULL) {
        if (isSuit(dp->d_name, argv[1])) {
            printf("%s\n", dp->d_name);
        }
    }

    closedir(dirp);
    return(0);
}

