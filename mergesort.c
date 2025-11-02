#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "mergesort.h"

extern int *A;
extern int *B;
extern int cutoff;

struct argument *buildArgs(int left, int right, int level) {
    struct argument *arg = malloc(sizeof(struct argument));
    arg->left = left;
    arg->right = right;
    arg->level = level;
    return arg;
}

void *parallel_mergesort(void *arg) {
    struct argument *a = (struct argument *)arg;
    int left = a->left;
    int right = a->right;
    int level = a->level;

    if (level >= cutoff) {
        mergesort(left, right);
        free(a);
        return NULL;
    }

    int mid = (left + right) / 2;
    pthread_t t1, t2;

    struct argument *leftArg = buildArgs(left, mid, level + 1);
    struct argument *rightArg = buildArgs(mid + 1, right, level + 1);

    pthread_create(&t1, NULL, parallel_mergesort, leftArg);
    pthread_create(&t2, NULL, parallel_mergesort, rightArg);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    merge(left, mid, mid + 1, right);

    free(a);
    return NULL;
}
