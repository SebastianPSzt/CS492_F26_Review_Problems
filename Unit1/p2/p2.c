#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>


/**
 * 0) Upon a call to fork(), what elements of the virtual memory image are shared between the parent and child? Explain.
 * 
 * 1) How many processes are there in total? (include the parent)
 * 
 * 2) List the prints produced by each process individually
 */
int main(int argc, char* argv[]) {
    pid_t p1;
    pid_t p2;

    uint8_t x = 0;

    if ((p1 = fork())) {
        x++;
    }

    printf("%u\n", x);

    if (p1 && (p2 = fork())) {
        x++;
    }

    printf("%u\n", x);

    return 0;
}