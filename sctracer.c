// Blue Hartsell
// CPSC 3220 F22
// project_1

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>

#define MAXSIZE 1024
#define SYSCALLNUM 326
#define true 1 // for while loop  readability

FILE *outfile; // global output file pointer

int systemcalls[SYSCALLNUM];

int main(int argc, char **argv) {
    // Fork
    pid_t childpid = fork();

    if (childpid==0) {
        // Inside child process

        int len = (argc-1) + 1; // length of spaces in argv + \n
        for (int i = 1; i < argc; i++) {
            len += strlen(argv[i]); // add length of each argument
        }

        // Create string of correct length
        char execstr[len];

        // Format arguments into execstr
        strcpy(execstr, argv[1]);
        for(int i = 2; i < argc-1; i++) { // start at second arg
            strcat(execstr, " ");
            strcat(execstr, argv[i]);
        }

        // setup ptrace
        ptrace(PTRACE_TRACEME);
        childpid = getpid();
        kill(childpid, SIGSTOP);

        // run program
        system(execstr);
    } else {
        int status;

        // wait for child to stop   
        waitpid(childpid, &status, 0);

        ptrace(PTRACE_SETOPTIONS, childpid, 0, PTRACE_O_TRACESYSGOOD);

        while(true) {
            do {
                // wait for syscall
                ptrace(PTRACE_SYSCALL, childpid, 0, 0);
                waitpid(childpid, &status, 0);

                // check if child exited
                if(WIFEXITED(status)) {
                    outfile = fopen(argv[argc-1], "w"); // open output file in write mode
                    
                    // print system calls
                    for(int i = 0; i < SYSCALLNUM; i++) {
                        if(systemcalls[i]) { // if call exists
                            fprintf(outfile, "%d\t%d\n", i, systemcalls[i]);
                        }
                    }

                    // close output file
                    fclose(outfile);
                    return 1; // success
                }
            } while(!(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80));

            // get syscall number
            int count = ptrace(PTRACE_PEEKUSER, childpid, sizeof(long) * ORIG_RAX, 0);
            systemcalls[count]++;
            
            // wait for syscall exit
            ptrace(PTRACE_SYSCALL, childpid, 0, 0);
            waitpid(childpid, &status, 0);
        }
    }
}