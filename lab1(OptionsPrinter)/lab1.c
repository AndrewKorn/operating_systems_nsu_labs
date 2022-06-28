#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <ulimit.h>
#include <stdlib.h>
#include <stdio.h>
extern char** environ;

int  main(int argc, char *argv[]) {
    char options[] = "ispuU:cC:dvV:";
    if (argc < 2) {
        fprintf(stderr, "please use given options %s\n", options);
    }
    struct rlimit coreLimit;
    char** env;
    int c;
    while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
            case 'i':
                printf("uid = %ld\neuid = %ld\ngid = %ld\negid = %ld\n", getuid(), geteuid(), getgid(), getegid());
                break;
            case 's':
                setpgid(0,0);
                break;
            case 'p':
                printf("pid = %ld\nppid = %ld\npgid = %ld\n", getpid(), getppid(), getpgid(0));
                break;
            case 'u':
                printf("ulimit = %ld\n", ulimit(UL_GETFSIZE));
                break;
            case 'U':
                ulimit(UL_SETFSIZE, atol(optarg));
                break;
            case 'c':
                getrlimit(RLIMIT_CORE, &coreLimit);
                printf("core size(soft) = %lu\ncore size(hard) = %lu\n", coreLimit.rlim_cur, coreLimit.rlim_max);
                break;
            case 'C':
                coreLimit.rlim_cur = atol(optarg);
                coreLimit.rlim_max = atol(optarg);
                setrlimit(RLIMIT_CORE, &coreLimit);
                break;
            case 'd':
                printf("current dir is: %s\n", getcwd(NULL, 100));
                break;
            case 'v':
                for (env = environ; *env; ++env) {
                    printf("%s\n", *env);
                }
                break;
            case 'V':
                putenv(optarg);
                break;
        }
    }
    return 0;
}



