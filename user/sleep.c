/*
Implement the UNIX program sleep for xv6; your sleep should pause for a
user-specified number of ticks. A tick is a notion of time defined by the xv6
kernel, namely the time between two interrupts from the timer chip.
*/
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "Usage: sleep ticks\n");
        exit(1);
    }

    int ticks = atoi(argv[1]);
    sleep(ticks);

    exit(0);
}