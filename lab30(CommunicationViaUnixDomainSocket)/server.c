#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    int sfd, cfd;
    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    char* socketPath = "./socket";
    strcpy(addr.sun_path, socketPath);
    unlink(socketPath);

    if (bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        return -1;
    }

    if (listen(sfd, 1) == -1) {
        perror("listen error");
        return -1;
    }

    while (1) {
        if ((cfd = accept(sfd, NULL, NULL)) == -1) {
            perror("accept error");
            return -1;
        }

        printf("the connection is established\n");
        int rc;
        char buf[BUFSIZ];

        while ((rc = read(cfd, buf, BUFSIZ)) > 0) {
            for (int i = 0; i < rc; ++i) {
                putchar(toupper(buf[i]));
            }
        }
        if (rc == -1) {
            perror("read error");
            return -1;
        }
        if (rc == 0) {
            printf("the connection was closed\n");
            close(cfd);
        }
    }
   
}
