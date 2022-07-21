#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void signal_handler(int signum);

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGILL, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGCHLD, signal_handler);

    int num;
    pid_t pid;

    for (int i = 0; i < 5; i++) {
        printf("Program running... Press CTRL+C or CTRL+\\\n");
        sleep(1);
    }

    printf("Enter a number to find reciprocal: ");
    scanf("%d", &num);
    if (num == 0)
        raise(SIGFPE);
    else
        printf("Reciprocal: %.2f\n", 1.0 / num);

    int arr[3];
    printf("Enter more than 3 elements: ");
    for (int i = 0; i < 5; i++) {
        if (i > 2)
            raise(SIGSEGV);
        else
            scanf("%d", &arr[i]);
    }

    pid = fork();
    if (pid < 0)
        printf("Error forking child\n");
    else if (pid == 0) {
        printf("In child process\n");
        return 0;
    } else {
        wait(NULL);
        printf("In the parent process\n");
        sleep(2);
    }

    raise(SIGILL);
    return 0;
}

void signal_handler(int signum) {
    switch (signum) {
        case SIGINT:
            printf("Caught an interrupt signal, signum is %d\n", signum);
            break;
        case SIGSEGV:
            printf("Caught a segmentation violation, signum is %d\n", signum);
            break;
        case SIGILL:
            printf("Caught an illegal instruction, signum is %d\n", signum);
            break;
        case SIGFPE:
            printf("Caught a floating point exception, signum is %d\n", signum);
            break;
        case SIGQUIT:
            printf("Caught a dump core signal, signum is %d\n", signum);
            break;
        case SIGCHLD:
            printf("Child status has changed, signum is %d\n", signum);
            wait(NULL);
            break;
        default:
            printf("Caught a signal, signum is %d\n", signum);
    }
}
