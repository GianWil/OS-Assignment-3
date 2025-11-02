
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "mergesort.h"


extern int *A;
extern int *B;
extern int cutoff;


void mergesort(int left, int right);
void merge(int leftstart, int leftend, int rightstart, int rightend);
struct argument *buildArgs(int left, int right, int level);
void *parallel_mergesort(void *arg);


void mergesort(int left, int right) {
if (left >= right) return;


int mid = left + (right - left) / 2;
mergesort(left, mid);
mergesort(mid + 1, right);
merge(left, mid, mid + 1, right);
}


void merge(int leftstart, int leftend, int rightstart, int rightend) {
int i = leftstart;
int j = rightstart;
int k = 0;


int left_len = leftend - leftstart + 1;
int right_len = rightend - rightstart + 1;


if (left_len <= 0 || right_len <= 0) return;


while (i <= leftend && j <= rightend) {
if (A[i] <= A[j]) {
B[k++] = A[i++];
} else {
B[k++] = A[j++];
}
}
while (i <= leftend) B[k++] = A[i++];
while (j <= rightend) B[k++] = A[j++];


memcpy(&A[leftstart], B, (size_t)k * sizeof(int));
}


struct argument *buildArgs(int left, int right, int level) {
struct argument *arg = (struct argument *)malloc(sizeof(struct argument));
if (arg == NULL) }
```
