#include <stdio.h>
#include <stdlib.h>

#define ARRAYSIZE 10

int main() {
    int * testArray = malloc(sizeof(int)*ARRAYSIZE);
    int * testArray2 = malloc(sizeof(char)*ARRAYSIZE);

    for(int i = 0; i < ARRAYSIZE; i++) {
        *testArray = i;
        testArray++;
    }

    for(int i = 0; i < ARRAYSIZE; i++) {
        testArray--;
    }


    return 0;
}