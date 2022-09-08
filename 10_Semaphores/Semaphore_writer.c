#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define SHM_KEY 0x12345
#define SEM_KEY 0x54321

typedef struct sh_mem_seg {
    int counter;
    int write_complete;
    int read_complete;
} ShMemSeg;

void shared_memory_counter_increment(int pid, ShMemSeg *mem_seg, int total_count);
void remove_semaphore();

int main(int argc, char *argv[]) {
    int shm_id;
    ShMemSeg *mem_seg;
    int total_count;
    pid_t pid;
    if (argc != 2)
        total_count = 10000;
    else {
        total_count = atoi(argv[1]);
        if (total_count < 10000)
            total_count = 10000;
    }
    printf("Total Count is %d\n", total_count);
    shm_id = shmget(SHM_KEY, sizeof(ShMemSeg), 0644 | IPC_CREAT);

    if (shm_id == -1) {
        perror("Shared memory");
        return 1;
    }

    mem_seg = shmat(shm_id, NULL, 0);

    if (mem_seg == (void *) -1) {
        perror("Shared memory attach: ");
        return 1;
    }
    mem_seg->counter = 0;
    pid = fork();

    if (pid == 0) {
        shared_memory_counter_increment(pid, mem_seg, total_count);
        return 0;
    } else if (pid > 0) {
        shared_memory_counter_increment(pid, mem_seg, total_count);
    } else {
        perror("Fork Failure\n");
        return 1;
    }

    while (mem_seg->read_complete != 1)
        sleep(1);

    if (shmdt(mem_seg) == -1) {
        perror("shmdt");
        return 1;
    }

    if (shmctl(shm_id, IPC_RMID, 0) == -1) {
        perror("shmctl");
        return 1;
    }
    printf("Writing Process: Complete\n");
    remove_semaphore();
    return 0;
}

void shared_memory_counter_increment(int pid, ShMemSeg *mem_seg, int total_count) {
    int counter;
    int num_times;
    int sleep_time;
    int sem_id;
    struct sembuf sem_buf;

    sem_id = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (sem_id >= 0) {
        printf("First Process\n");
        sem_buf.sem_num = 0;
        sem_buf.sem_op = 1;
        sem_buf.sem_flg = 0;
        if (semop(sem_id, &sem_buf, 1) == -1) {
            perror("Semaphore Operation: ");
            return;
        }
    } else if (errno == EEXIST) { // Already other process got it
        printf("Second Process\n");
        sem_id = semget(SEM_KEY, 1, 0);
        if (sem_id < 0) {
            perror("Semaphore GET: ");
            return;
        }

        /* Waiting for the resource */
        sem_buf.sem_num = 0;
        sem_buf.sem_op = 0;
        sem_buf.sem_flg = SEM_UNDO;
        if (semop(sem_id, &sem_buf, 1) == -1) {
            perror("Semaphore Locked: ");
            return;
        }
    }
    sem_buf.sem_num = 0;
    sem_buf.sem_op = -1; /* Allocating the resources */
    sem_buf.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_buf, 1) == -1) {
        perror("Semaphore Locked: ");
        return;
    }
    counter = mem_seg->counter;
    mem_seg->write_complete = 0;
    if (pid == 0)
        printf("SHM_WRITE: CHILD: Now writing\n");
    else if (pid > 0)
        printf("SHM_WRITE: PARENT: Now writing\n");

    for (num_times = 0; num_times < total_count; num_times++) {
        counter += 1;
        mem_seg->counter = counter;
        /* Sleeping for a second for every thousand */
        sleep_time = counter % 1000;
        if (sleep_time == 0)
            sleep(1);
    }
    mem_seg->write_complete = 1;
    sem_buf.sem_op = 1; /* Releasing the resource */
    if (semop(sem_id, &sem_buf, 1) == -1) {
        perror("Semaphore Locked\n");
        return;
    }

    if (pid == 0)
        printf("SHM_WRITE: CHILD: Writing Done\n");
    else if (pid > 0)
        printf("SHM_WRITE: PARENT: Writing Done\n");
}

void remove_semaphore() {
    int sem_id = semget(SEM_KEY, 1, 0);
    if (sem_id < 0) {
        perror("Remove Semaphore: Semaphore GET: ");
        return;
    }

    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("Remove Semaphore: Semaphore CTL: ");
        return;
    }
}
