#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define STRING_SIZE 512

typedef struct msg_buf {
    long type;
    char text[STRING_SIZE];
} MsgBuf;

void terminate(char *s);

int main() {
    int msg_id;
    int msg_flg = IPC_CREAT | 0666;
    key_t key;
    MsgBuf send_buf1, send_buf2;
    size_t buf_len1, buf_len2;

    key = 1234;

    if ((msg_id = msgget(key, msg_flg)) < 0) {
        printf("\nerrno %d\n", errno);
        terminate("msgget");
    }

    send_buf1.type = 1;
    printf("Enter a message to add to message queue: ");
    scanf("%[^\n]", send_buf1.text);
    getchar();
    buf_len1 = strlen(send_buf1.text) + 1;

    send_buf2.type = 2;
    printf("Enter another message to add to message queue : ");
    scanf("%[^\n]", send_buf2.text);
    getchar();
    buf_len2 = strlen(send_buf2.text) + 1;

    if (msgsnd(msg_id, &send_buf1, buf_len1, IPC_NOWAIT) < 0) {
        printf("%d, %ld, %s, %zu\n", msg_id, send_buf1.type, send_buf1.text, buf_len1);
        terminate("msgsnd");
    }
    printf("Message 1 sent\n");

    if (msgsnd(msg_id, &send_buf2, buf_len2, IPC_NOWAIT) < 0) {
        printf("%d, %ld, %s, %zu\n", msg_id, send_buf2.type, send_buf2.text, buf_len2);
        terminate("msgsnd");
    }
    printf("Message 2 sent\n");

    return 0;
}

void terminate(char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}
