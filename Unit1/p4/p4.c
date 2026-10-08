#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

void* helper(void* param) {
    *(int*)(param) += 1;
    printf("%d\n", *(int*)param);
    pthread_exit(0);
}

/**
 * 0) What are some differences between processes and threads?
 * 
 * 1) What values are printed by the program below? Assume atomicity per line of C code.
 * 
 * 2) We would like to truly parallelize our program. 25% of our program is parallelizable. We have 4 CPU cores at our disposal.
 * Please calculate the CPU speedup.
 * 
 */
int main(void) {
    pthread_t t1;
    pthread_t t2;

    int* np = (int*)malloc(sizeof(int));
    *np = 0;


    pthread_create(&t1, NULL, helper, (void*)np);
    pthread_create(&t2, NULL, helper, (void*)np);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    *np += 1;

    printf("%d\n", *np);

    free(np);
    exit(EXIT_SUCCESS);
}