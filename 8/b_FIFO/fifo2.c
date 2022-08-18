// C program to implement one side of FIFO
// This side reads first, then writes
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define STRING_SIZE 512

int main() {
    int fd;
    char str1[STRING_SIZE], str2[STRING_SIZE];

    char *fifo = "/tmp/fifo";
    mkfifo(fifo, 0666);

    printf("Waiting for process 1 to connect\n");
    for (int i = 0; i < 3; i++) {
        fd = open(fifo, O_RDONLY);
        printf("Waiting for other process to write\n");
        read(fd, str1, STRING_SIZE);
        printf("Process 1: %s\n", str1);
        close(fd);

        fd = open(fifo, O_WRONLY);
        printf(">>> ");
        fgets(str2, STRING_SIZE, stdin);
        write(fd, str2, strlen(str2) + 1);
        close(fd);
    }

    return 0;
}
