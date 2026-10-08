#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

/**
 * 0) Explain what execvp() and wait() do
 * 
 * 1) How many processes are spawned during the execution of this program? (include the parent)
 * 
 * 2) Please give the independent strings printed by each process
 * 
 * 3) Bonus: what do the suffixes of exec signify? i.e. [v, l] and [p, e]
 */
int main(void) {
    pid_t p1;
    pid_t p2;
    
    int n = 0;

    if ((p1 = fork())) {
        n++;
    }

    printf("%d\n", n);

    if (!p1) {
        p2 = fork();
        n += 2;
    }

    if (!p2) {
        execvp("./helper", NULL);
    }

    if (p1) {
        wait(NULL);
        printf("%d\n", n);
    } else {
        wait(NULL);
        printf("%d\n", n);
    }

    exit(EXIT_SUCCESS);
}