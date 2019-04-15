#include <stdio.h>
#include <unistd.h>


int main() {
    printf("--beginning of program\n");

    int counter = 0;
    pid_t pid = fork();
    printf("pid = %d\n", pid);
    if (pid < 0) {
        // fork failed
        printf("fork()  falied\n");
        return 1;
    }

    if (pid == 0) {
        // child process
        for(int i = 0; i < 15; i++) {
            printf("child process: counter = %d\n", ++counter);
        }
        return 0;
    }

    // parent process
    for(int i = 0; i < 5; i++) {
        printf("parent process: counter = %d\n", ++counter);
    }

    return 0;
}