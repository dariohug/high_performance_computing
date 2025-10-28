#include "stdio.h"
#include "math.h"

int main (int argc, char** argv) {
    // Leibniz Formula: 
    // Pi/4 = sum, n = 0 -> inf ((-1)^n)/2n + 1

    // 10 ** 6 to be kind to eiger here...
    int n = (int)pow(10, 6);

    long double quarter_pi = 0; 

    printf("started\n");

    for (int i = 0; i < n; i++) {
        quarter_pi += pow(-1, i) / (2 * i + 1);

        if (i % 10000 == 0) {
            printf("Step %i of %i \n", i, n);
        }
    }
    
    printf("Pi is %.15Lf\n", quarter_pi * 4.0L);

    return 0;

}