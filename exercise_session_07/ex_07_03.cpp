#include <iostream>

int ii(int x, int y){
    return x * 14 + y;
}

int main () {
    int x,y; 
    for (x = 0; x<14; x++) {
        for (y = 0; y<14; y++){
            printf("(%d, %d) %d\n", x, y, ii(x,y));
        }
    }


}