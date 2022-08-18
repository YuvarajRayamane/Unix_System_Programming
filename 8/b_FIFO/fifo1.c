// C program to implement one side of FIFO
// This side writes first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define STRING_SIZE 512

int main() {
    int fd;
    char arr1[STRING_SIZE], arr2[STRING_SIZE];

    char *fifo = "/tmp/fifo";
    mkfifo(fifo, 0666);
    printf("FIFO file created\n");

    printf("Waiting for process 2 to connect\n");
    for (int i = 0; i < 3; i++) {
        fd = open(fifo, O_WRONLY);
        printf(">>> ");
        fflush(stdout);
        fgets(arr2, STRING_SIZE, stdin);
        write(fd, arr2, strlen(arr2) + 1);
        close(fd);

        printf("Waiting for other process to write\n");
        fd = open(fifo, O_RDONLY);
        read(fd, arr1, sizeof(arr1));
        printf("Process 2: %s\n", arr1);
        close(fd);
    }

    return 0;
}
