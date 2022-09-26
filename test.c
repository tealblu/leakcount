#include <stdio.h>
#include <stdlib.h>

#define ARRAYSIZE 10

int main() {
    int * testArray = malloc(sizeof(int)*ARRAYSIZE);

    for(int i = 0; i < ARRAYSIZE; i++) {
        *testArray = i;
        testArray++;
    }

    for(int i = 0; i < ARRAYSIZE; i++) {
        printf("%i\n", *testArray);
        testArray--;
    }


    return 0;
}