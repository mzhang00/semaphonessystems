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

union semun {
    int val;
    struct semid_ds * buf;
    unsigned short * array;
    struct seminfo * __buf;
};

int main(int argc, char * argv[]) {
    int semd, shmd, v, r, file;

    if (argc == 1){
        printf("Please enter a valid command!\n");
    }else{
        if(strcmp(argv[1], "-c") == 0){
            semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
            union semun us;
            us.val = 1;
            semctl(semd, 0, SETVAL, us);
            printf("semaphore created\n");

            shmd = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0644);
            printf("shared memory created\n");

            file = open("story.txt",  O_CREAT | O_TRUNC | O_RDWR, 0644);
            printf("file created\n");
            close(file);
        }else if(strcmp(argv[1], "-v") == 0){
            file = open("story.txt", O_RDONLY);
            char line [SEG_SIZE];
            printf("The story so far:\n");

            while (read(file, line, SEG_SIZE) > 0) {
                printf("%s\n",line);
            }
            close(file);
        }else if(strcmp(argv[1], "-r") == 0){
            printf("trying to get in\n");
            
            struct sembuf sb;
            sb.sem_num = 0;
            sb.sem_op = -1;

            file = open("story.txt", O_RDONLY);
            char line [SEG_SIZE];
            printf("The story so far:\n");

            while (read(file, line, SEG_SIZE) > 0) {
                printf("%s\n",line);
            }
            close(file);

            printf("\n");
            
            shmctl(shmd, IPC_RMID, 0);
            printf("shared memory removed\n");

            remove("story.txt");
            printf("file removed\n");
                        
            semop(semd, &sb, 1);
            semctl(semd, IPC_RMID, 0);
            printf("semaphore removed\n");
        }else{
            printf("Please enter a valid command!\n");
        }
    }
    return 0;
}