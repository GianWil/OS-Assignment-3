/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include <pthread.h>
#include "mergesort.h"

/* helper to compute mid without overflow */
static inline int mid_of(int l, int r) {
    return l + (r - l) / 2;
}

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){
    int i = leftstart;
    int j = rightstart;
    int k = leftstart;

    /* merge into B[leftstart..rightend] */
    while (i <= leftend && j <= rightend) {
        if (A[i] <= A[j]) {
            B[k++] = A[i++];
        } else {
            B[k++] = A[j++];
        }
    }
    while (i <= leftend) {
        B[k++] = A[i++];
    }
    while (j <= rightend) {
        B[k++] = A[j++];
    }

    /* copy merged run back to A */
    memcpy(&A[leftstart], &B[leftstart], (size_t)(rightend - leftstart + 1) * sizeof(int));
}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right){
    if (left >= right) return;

    int m = mid_of(left, right);
    my_mergesort(left, m);
    my_mergesort(m + 1, right);
    merge(left, m, m + 1, right);
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
    struct argument *a = (struct argument *)malloc(sizeof(struct argument));
    if (!a) {
        perror("malloc");
        exit(1);
    }
    a->left = left;
    a->right = right;
    a->level = level;
    return a;
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){
    struct argument *args = (struct argument *)arg;
    int left = args->left;
    int right = args->right;
    int level = args->level;

    if (left >= right) {
        /* nothing to do for size <= 1 */
        return NULL;
    }

    /* if we've reached the cutoff, do serial mergesort */
    if (level >= cutoff) {
        my_mergesort(left, right);
        return NULL;
    }

    /* otherwise, split and sort halves in parallel threads */
    int m = mid_of(left, right);

    pthread_t t1, t2;
    struct argument *leftArg = buildArgs(left, m, level + 1);
    struct argument *rightArg = buildArgs(m + 1, right, level + 1);

    if (pthread_create(&t1, NULL, parallel_mergesort, (void *)leftArg) != 0) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&t2, NULL, parallel_mergesort, (void *)rightArg) != 0) {
        perror("pthread_create");
        exit(1);
    }

    /* wait for both halves to finish */
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    /* parent (creator) frees child arg structs after join */
    free(leftArg);
    free(rightArg);

    /* now merge the two sorted halves */
    merge(left, m, m + 1, right);

    /* IMPORTANT: do NOT free 'args' here.
       The owner (the creator of this thread) frees its own arg after join.
       In the top-level call (no thread created), main() frees it. */
    return NULL;
}
