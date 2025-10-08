#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "get_time.h"

typedef struct {
    float x;
    float y;
    float z;
    float r2;
} coord;

int main(int argc, char *argv[]) {
    const int N = 1024*1024*256;
    coord *data;
    int i;
    double sum;
    double start_time, elapsed_time; 

    start_time = getTime();

    data = malloc(N * sizeof(coord));
    assert(data);
    
    sum = 0.0;
    for(i=0; i<N; ++i) {
        data[i].x = i & 31;
        data[i].y = i & 63;
        data[i].z = i & 15;
        data[i].r2 = data[i].x*data[i].x + data[i].y*data[i].y + data[i].z*data[i].z;
        sum += sqrt(data[i].r2);
    }
    printf("sum=%f\n", sum);
    elapsed_time = getTime() - start_time; 
    printf("Elapsed Time: %.4f\n", elapsed_time);
    return 0;
}
