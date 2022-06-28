#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
    struct termios tty, savetty;

    if (!isatty(fileno(stdin))) {
        perror("not a terminal");
        return 1;
    }

    if (tcgetattr(fileno(stdin), &tty) == -1) {
        perror("tcgetattr");
        return 1;
    }

    savetty = tty;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;
    tty.c_lflag |= IEXTEN;
    tty.c_lflag &= ~(ISIG | ICANON | ECHO);

    if (tcsetattr(fileno(stdin), TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        return 1;
    }

    char line[40];
    int position = 0;
    char c;
    while (read(fileno(stdin), &c, 1) > 0) {
        if (c == savetty.c_cc[VEOF]) {
            if (position == 0) {
                break;
            }
        }
        else if (c == tty.c_cc[VERASE]) {
            if (position > 0) {
                write(fileno(stdout), "\b \b", 3);
                position -= 1;
            }
        }
        else if (c == tty.c_cc[VKILL]) {
            while(position > 0) {
                write(fileno(stdout), "\b \b", 3);
                position -= 1;
            }
        }
        else if (c == tty.c_cc[VWERASE]) {
            while (position > 0 && isspace(line[position - 1])) {
                write(fileno(stdout), "\b \b", 3);
                position -= 1;
            }
            while (position > 0 && !isspace(line[position - 1])) {
                write(fileno(stdout), "\b \b", 3);
                position -= 1;
            }
        }
        else if (c == '\n') {
            write(fileno(stdout), "\n", 1);
            write(fileno(stdout), "Edited line: ", 14);
            write(fileno(stdout), line, position);
            write(fileno(stdout), "\n", 1);
            position = 0;
        }
        else if (!isprint(c)) {
            write(fileno(stdout), "\07", 1);
        }
        else {
            write(fileno(stdout), &c, 1);
            line[position] = c;
            position += 1;
        }

        if (position > 40) {
            char new_line[40];
            int lastPosition = position;
            while (position > 0 && !isspace(line[position - 1])) {
                position -= 1;
            }


            if (position > 0) {
                int newLinePosition = 0;
                for (int i = position; i < lastPosition; ++i) {
                    write(fileno(stdout), "\b \b", 3);
                    new_line[newLinePosition] = line[i];
                    newLinePosition += 1;
                }

                write(fileno(stdout), "\n", 1);
                write(fileno(stdout), "Edited line: ", 14);
                write(fileno(stdout), line, position);
                position = newLinePosition;

                for (int i = 0; i < newLinePosition; ++i) {
                    line[i] = new_line[i];
                }
                write(fileno(stdout), "\n", 1);



                for (int i = 0; i < position; i++) {
                    write(fileno(stdout), &line[i], 1);
                }
            }
            else {
                write(fileno(stdout), "\n", 1);
                write(fileno(stdout), "Edited line: ", 14);
                write(fileno(stdout), line, lastPosition);
                write(fileno(stdout), "\n", 1);
            }
        }
    }

    tcsetattr(fileno(stdin), TCSANOW, &savetty);

    return 0;
}


