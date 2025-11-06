#include <iostream>
using namespace std;

int main() {
    // Initialize the Matrix
    int** M = new int*[2];
    *(M + 0) = new int[5]{0, 1, 2, 3, 4};
    *(M + 1) = new int[5]{5, 6, 7, 8, 9};

    // Perform swap
    int** M2 = new int*[2];
    for (int r = 0; r < 2; r++) {
        *(M2 + r) = new int[5];
        for (int c = 0; c < 5; c++) {
            *(*(M2+r)+c) = *(*(M+1-r)+4-c);
        }
    }

    // Print result, I used brakets for printing... hopefully ok 
    cout << "ORiginal: " << endl;
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 5; c++) {
            cout << M[r][c] << " ";
        }
        cout << endl;
    }
    cout << "Flipped: " << endl;
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 5; c++) {
            cout << M2[r][c] << " ";
        }
        cout << endl;
    }

    // Free memory
    delete[] *(M + 0);
    delete[] *(M + 1);
    delete[] M;

    delete[] *(M2 + 0);
    delete[] *(M2 + 1);
    delete[] M2;

    return 0;
}
