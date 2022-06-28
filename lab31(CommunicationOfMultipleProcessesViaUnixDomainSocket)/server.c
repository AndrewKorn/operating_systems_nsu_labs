#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <poll.h>

#define CLIENT_NUMBER 10


int main() {
    int sfd;
    if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        return -1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    char *socketPath = "./socket";
    strcpy(addr.sun_path, socketPath);
    unlink(socketPath);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("bind error");
        return -1;
    }

    if (listen(sfd, 5) == -1) {
        perror("listen error");
        return -1;
    }

    struct pollfd fds[CLIENT_NUMBER + 1];
    int nfds = 1;
    int timeout = 1000 * 10 * 60;
    fds[0].fd = sfd;
    fds[0].events = POLLIN;

    while (1) {
        int ret = poll(fds, nfds, timeout);

        if (ret == -1) {
            perror("poll");
            break;
        }
        if (ret == 0) {
            perror("poll timeout");
            break;
        }

        int size = nfds;
        for (int i = 0; i < size; ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == sfd) {
                    if (nfds < CLIENT_NUMBER + 1) {
                        int cfd;
                        if ((cfd = accept(sfd, NULL, NULL)) == -1) {
                            perror("accept error");
                            return -1;
                        }
                        printf("the connection is established\n");
                        fds[nfds].fd = cfd;
                        fds[nfds].events = POLLIN;
                        ++nfds;
                    }
                }
                else {
                    int rc;
                    char buf[BUFSIZ];

                    rc = read(fds[i].fd, buf, BUFSIZ);
                    if (rc  > 0) {
                        for (int j = 0; j < rc; ++j) {
                            putchar(toupper(buf[j]));
                        }
                    }

                    if (rc == -1) {
                        perror("read error");
                        return -1;
                    }
                    if (rc == 0) {
                        printf("the connection was closed\n");
                        close(fds[i].fd);
                        for (int j = i; j < nfds - 1; ++j) {
                            fds[j] = fds[j + 1];
                        }
                        --nfds;
                    }
                }
            }
        }
    }
    return 0;
}

