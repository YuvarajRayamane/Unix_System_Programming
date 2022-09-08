#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 0x12345

typedef struct sh_mem_seg {
    int counter;
    int write_complete;
    int read_complete;
} ShMemSeg;

int main() {
    int shm_id;
    ShMemSeg *mem_seg;

    shm_id = shmget(SHM_KEY, sizeof(ShMemSeg), 0644 | IPC_CREAT);

    if (shm_id == -1) {
        perror("Shared memory");
        return 1;
    }
    mem_seg = shmat(shm_id, NULL, 0);

    if (mem_seg == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }

    while (mem_seg->write_complete != 1) {
        if (mem_seg->counter == -1) {
            perror("read");
            return 1;
        }
        sleep(1);
    }
    printf("Reading Process: Shared Memory: Counter is %d\n", mem_seg->counter);
    printf("Reading Process: Reading Done, Detaching Shared Memory\n");
    mem_seg->read_complete = 1;

    if (shmdt(mem_seg) == -1) {
        perror("shmdt");
        return 1;
    }
    printf("Reading Process: Complete\n");
    return 0;
}
