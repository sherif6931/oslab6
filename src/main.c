#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

pid_t child2_pid;

void sigchld_handler(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid <= 0) return;

    if (pid != child2_pid) {
        int exit_code = WEXITSTATUS(status);
        if (exit_code == 1) {
            kill(child2_pid, SIGKILL);
        } else {
            kill(child2_pid, SIGCONT);
        }
    } else {
        exit(0);
    }
}

int main() {
    setbuf(stdout, NULL);
    signal(SIGCHLD, sigchld_handler);

    pid_t child1_pid = fork();
    if (child1_pid == 0) {
        printf("Child1 PID: %d Parent PID: %d\n", getpid(), getppid());
        sleep(1);
        int value;
        scanf("%d", &value);
        exit(value);
    }

    child2_pid = fork();
    if (child2_pid == 0) {
        printf("Child2 PID: %d Parent PID: %d\n", getpid(), getppid());
        fflush(stdout);
        raise(SIGSTOP);
        printf("child process is resumed\n");
        exit(0);
    }

    while (1) pause();
}