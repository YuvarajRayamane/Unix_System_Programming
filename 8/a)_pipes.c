#include<stdio.h>
#include<unistd.h>

#define STRING_SIZE 512

int main() {
    int pipe_fds1[2], pipe_fds2[2];
    char read_message[STRING_SIZE];
    char pipe1_write_message[STRING_SIZE] = "Hi";
    char pipe2_write_message[STRING_SIZE] = "Hello";

    if (pipe(pipe_fds1) == -1) {
        printf("Unable to create pipe 1\n");
        return 1;
    }

    if (pipe(pipe_fds2) == -1) {
        printf("Unable to create pipe 2\n");
        return 1;
    }


    if (fork() > 0) {
        close(pipe_fds1[0]);
        close(pipe_fds2[1]);

        printf("In parent process, writing \"%s\" to pipe 1\n", pipe1_write_message);
        write(pipe_fds1[1], pipe1_write_message, sizeof(pipe1_write_message));

        read(pipe_fds2[0], read_message, sizeof(read_message));
        printf("In parent process, reading \"%s\" from pipe 2\n", read_message);
    } else {
        close(pipe_fds1[1]);
        close(pipe_fds2[0]);

        read(pipe_fds1[0], read_message, sizeof(read_message));
        printf("In child process, reading \"%s\" from pipe 1\n", read_message);

        printf("In child process, writing \"%s\" to pipe 2\n", pipe2_write_message);
        write(pipe_fds2[1], pipe2_write_message, sizeof(pipe2_write_message));
    }

    return 0;
}
