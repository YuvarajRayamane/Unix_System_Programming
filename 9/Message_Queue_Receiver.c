#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define STRING_SIZE 512

typedef struct msg_buf {
    long type;
    char text[STRING_SIZE];
} MsgBuf;

void terminate(char *s);

int main() {
    int msg_id;
    key_t key;
    MsgBuf rcv_buf;

    key = 1234;

    if ((msg_id = msgget(key, 0666)) < 0)
        terminate("msgget");

    if (msgrcv(msg_id, &rcv_buf, STRING_SIZE, 1, 0) < 0)
        terminate("msgrcv");
    printf("%s\n", rcv_buf.text);

    if (msgrcv(msg_id, &rcv_buf, STRING_SIZE, 2, 0) < 0)
        terminate("msgrcv");
    printf("%s\n", rcv_buf.text);

    return 0;
}

void terminate(char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}
