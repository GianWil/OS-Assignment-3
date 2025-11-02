#ifndef MERGESORT_H
#define MERGESORT_H

extern int *A;
extern int *B;
extern int cutoff;

struct argument {
    int left;
    int right;
    int level;
};

void mergesort(int left, int right);
void merge(int leftstart, int leftend, int rightstart, int rightend);
void *parallel_mergesort(void *arg);
struct argument *buildArgs(int left, int right, int level);

#endif
