#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    if (fork() == 0){
        pid_t process_id = getpid();
        pid_t parent_id = getppid();
        char command[1000];

        printf("Hello from the child process\n");
        printf("Child process ID: %d\n", process_id);
        printf("Parent process ID: %d\n", parent_id);

        printf("Enter a command: ");
        scanf("%[^\n]s", command);

        printf("Executing the command: \n\n");
        system(command);
        printf("\n");
    } else {
        pid_t child_id = wait(NULL);
        printf("Hello from the parent process\n");
        printf("Parent process ID: %d\n", getpid());
        printf("Child process ID was: %d\n", child_id);
    }
}
