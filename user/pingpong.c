/*
Write a program that uses UNIX system calls to 'ping-pong' a byte between two
processes over a pair of pipes, one for each direction. The parent should send a
byte to the child; the child should print "<pid>: received ping", where <pid> is
its process ID, write the byte on the pipe to the parent, and exit; the parent
should read the byte from the child, print "<pid>: received pong", and exit.
*/
#include "user/user.h"

#define RD 0
#define WR 1

int main(int argc, char *argv[]) {
    int fdp2c[2], fdc2p[2];
    char strp2c[8] = "ping", strc2p[8] = "pong";
    char buf[8];
    if (pipe(fdc2p) < 0 || pipe(fdp2c) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }
    if (fork() == 0) {
        if (read(fdp2c[RD], buf, strlen(strp2c)) < 0) {
            fprintf(2, "Child read() failed\n");
            exit(1);
        }
        printf("%d: received %s\n", getpid(), buf);
        if (write(fdc2p[WR], strc2p, strlen(strc2p)) < 0) {
            fprintf(2, "Child write() failed\n");
            exit(1);
        }
    } else {
        if (write(fdp2c[WR], strp2c, strlen(strp2c)) < 0) {
            fprintf(2, "Parent write() failed\n");
            exit(1);
        }
        wait(0);  // 等待子进程结束
        if (read(fdc2p[RD], buf, strlen(strc2p)) < 0) {
            fprintf(2, "Parent read() failed\n");
            exit(1);
        }
        printf("%d: received %s\n", getpid(), buf);
    }

    exit(0);
}