#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include "../incl/_rint.h"

typedef struct job {
    pid_t p;
    int status;
    int interrupt;
    int exit_code;
} p_job;

p_job child2;

void sigchld_handler(int SIG) {
    (void) SIG; // 🤷‍♂️

    p_job proc;

    proc.p = waitpid(-1, &proc.status, WNOHANG | WUNTRACED);
    
    if (proc.p <= 0) return;

    if (proc.p != child2.p) {

        if (WIFEXITED(proc.status)) {
            proc.exit_code = WEXITSTATUS(proc.status);

            proc.interrupt = 0;
        } else if (WIFSIGNALED(proc.status)) {
            proc.exit_code = -1;
            
            proc.interrupt = WTERMSIG(proc.status);
        } else return;

        #ifdef SIGFLAG
        sig_write(proc.interrupt);
        #endif
        
    } else {
        if (WIFEXITED(proc.status) || WIFSIGNALED(proc.status)) {
            
            _exit(0);
        }
        
        return;
    }

    if (proc.exit_code == 1) {
        
        kill(child2.p, SIGKILL);
    } else {
     
        kill(child2.p, SIGCONT);
    }
}

int main() {
    setbuf(stdout, NULL);
    signal(SIGCHLD, sigchld_handler);

    if (sig_init() != 0) {
        perror("pipe");

        exit(1);
    } 

    p_job _read;
    
    _read.p = fork();
    if (_read.p == 0) {
        while (sig_read(&_read.status) > 0) {

            fprintf(stderr, "[INTERRUPT=%d]\n", _read.status);
        }

        _exit(0);
    }

    p_job child1;

    child1.p = fork();
    if (child1.p == 0) {
        printf("Child1 PID: %d Parent PID: %d\n\n", getpid(), getppid());
        sleep(5);

        int x;
        if (scanf("%d", &x) != 1) { 

            x = 0;
        }

        child1.exit_code = x;

        _exit(child1.exit_code);
    }

    child2.p = fork();
    if (child2.p == 0) {
        printf("Child2 PID: %d Parent PID: %d\n\n", getpid(), getppid());

        fflush(stdout); raise(SIGSTOP);
        printf("Child process resumed\n");

        _exit(0);
    }

    while (1) { 
    
        // A small job, in a small world.
    }

    return 0;
}