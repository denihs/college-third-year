#include <stdio.h>
#include <unistd.h>


int main() {
    pid_t pidA = fork();

    if (pidA == 0) {
        printf("B - %d - %d\n", getpid(), getppid());
        return 0;
    }

    pid_t pidB = fork();


    printf("A - %d\n", getpid(), getppid());

    return 0;
}