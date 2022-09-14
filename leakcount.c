// Blue Hartsell
// CPSC 3220 F22
// project_1

#include <stdio.h>
#include <stdlib.h>

int leakcount;

int main(int argc, char *argv[]) {
    // setup
    leakcount = 0;

    // set up LD_PRELOAD
    const char *ldp = "LD_PRELOAD=./memory_shim.so";

    // calculate length of exec string
    int len = strlen(ldp) + (argc-1) + 1; // length of preload + spaces in argv + \n
    for (int i = 1; i < argc; i++) {
        len += strlen(argv[i]); // add length of each argument
    }
    char execstr[len];

    // copy ldpreload into execstr
    strcpy(execstr, ldp);

    // copy arguments into execstr with spaces
    for (int i = 1; i < argc; i++) {
        strcat(execstr, " ");
        strcat(execstr, argv[i]);
    }

    // fork and exec
    pid_t p = fork();
    if (p = 0) {
        // child - run program
    } else {
        // parent - monitor program
    }

    // print results
}