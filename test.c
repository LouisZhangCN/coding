#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void share_memory_case(void);


int main(int argc, char **argv)
{
    share_memory_case();

    return 0;
}


void share_memory_case(void)
{
    int mem_size = 2 * 4 * 1024;
    int shm_id = 0;
    char *mem = NULL;
    struct shmid_ds stat;
    pid_t pid;
    int exit_code = 0;

    shm_id = shmget(IPC_PRIVATE, mem_size, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR);

    pid = fork();

    if (pid == 0) {
        mem = shmat(shm_id, NULL, 0 );
        printf("mem id:%d mem addr:%p mem size:%d\n",shm_id, mem, mem_size);

        shmctl(shm_id, IPC_STAT, &stat);

        int segsize;
        segsize = stat.shm_segsz;
        printf("segment size:%d\n", segsize);

        sprintf(mem, "Hello beauty");

        shmdt(mem);
        exit_code = 33;
        exit(exit_code);
    }
    else {
        int pid_stat;
        pid_t c_pid;
        c_pid = wait(&pid_stat);
        if (WIFEXITED(pid_stat))
            printf("child pid:%d exit with code:%d\n", c_pid, WEXITSTATUS(pid_stat));
        else {
            printf("child pid:%d exit failed\n", c_pid);
        }

        mem = shmat(shm_id, NULL, 0);
        printf("mem addr:%p\n", mem);

        printf("mem store:%s\n", mem);
        shmdt(mem);

        shmctl(shm_id, IPC_RMID, 0);
    }
}
