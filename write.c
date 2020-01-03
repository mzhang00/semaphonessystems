#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define SEM_KEY 24602
#define SHM_KEY 24601
#define SEG_SIZE 200

int main() {
    int semd, shmd, v, r, file;

    printf("trying to get in\n");

    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    semd = semget(SEM_KEY, 1, 0);
    semop(semd, &sb, 1);

    shmd = shmget(SHM_KEY, SEG_SIZE, 0644);
    char * data = shmat(shmd, 0, 0);
    
    printf("Last addition: %s\n", data);
    printf("Your addition: ");
    fgets(data, SEG_SIZE, stdin);

    file = open("story.txt", O_WRONLY | O_APPEND);
    write(file, data, SEG_SIZE);
    close(file);
    sb.sem_op = 1;
    semop(semd, &sb, 1);
    return 0;
}