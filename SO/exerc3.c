#include <stdio.h>
#include <unistd.h>


int main() {
    printf("--beginning of program\n");

    int counter = 0;
    pid_t pid = fork();

    if (pid < 0) {
        // fork failed
        printf("fork()  falied\n");
        return 1;
    }

    if (pid == 0) {
        pid_t pid2 = fork();

        if (pid2 < 0) {
            // fork failed
            printf("fork()  falied\n");
            return 1;
        }

        if (pid2 == 0) {

            pid_t grandchildPid = getpid();
            printf("grandchild pid %d, PID = %d\n", grandchildPid, pid2);

            printf("Neto foi criado\n");
            for(int i = 300; i < 305; i++) {
                printf("grandchild counter = %d\n", i);
                sleep(2);
            }
            printf("Neto vai morrer\n");
           return 0; 
        }
        // child process
        pid_t childPid = getpid();
        printf("child pid %d, PID = %d\n", childPid, pid2);
        printf("Filho foi criado\n");
        for(int i = 100; i < 105; i++) {
            printf("child counter = %d\n", i);
            sleep(1);
        }
        printf("Filho vai morrer\n");
        return 0;
    }

    // parent process
    pid_t parentPid = getpid();
    printf("Parent pid %d - PID = %d\n", parentPid, pid);
    for(int i = 1; i < 5; i++) {
        printf("parent counter = %d\n", i);
        sleep(2);
    }
    printf("Processo pai vai morrer\n");

    return 0;
}