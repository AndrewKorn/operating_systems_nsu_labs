#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    int sfd;
    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    char* socketPath = "./socket";
    strcpy(addr.sun_path, socketPath);

    if (connect(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        return -1;
    }

    int rc;
    char buf[BUFSIZ];
    while ((rc = read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
        if (write(sfd, buf, rc) != rc) {
            perror("writing error");
            return -1;
        }
    }

    close(sfd);
    return 0;
}

