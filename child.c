#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<errno.h>
#include "config.h"
#include "data.h"

#define key "/mysem1212"
#define shmkey 0x91919191
#define reskey 0x11225566
struct resDescriptor *rptr;
int SemID;
int main(int *argc, char *argv[])
{
    int shmid,semid,resid;
    int *count;
    int nograb = 1;
    int resnum,reqtime,instance;
    //sem_t *sem;
    //key_t key =  ftok("/mysem1212");
    //sem = sem_open(key,O_CREAT | 0666);
    /*if(sem == SEM_FAILED)
        perror("\n Error opening Semaphore");
    //sem_init(sem,0,0);
    if(sem = SEM_FAILED)
     perror("\n Semaphore not opened"); */
     int SemKey = ftok(key,43);
     if ((SemID = semget(SemKey, 1, 0600 | IPC_CREAT)) == -1) {
		perror("semget Err resource");
        exit(1);
    }
    shmid = shmget(shmkey,sizeof(int), IPC_CREAT | 0777);
    if(shmid ==-1)
    {
        perror("\n SHM Error");
    }
    else printf("\n SHM attached to CHild");
    count = shmat(shmid,0,0);
    if((resid = shmget(reskey, sizeof(struct resDescriptor), IPC_CREAT | 0777)) == -1)
    {
        perror("\n Resource Descriptor not created\n");
    }
    rptr = shmat(resid,0,0);


    srand(time(NULL));
    reqtime = (rand() % 251) * 1000;
    instance = rand() % 10;
    printf("\nEntering Critical section\n");
    sem_wait(SemID);
    //sleep(1);
    usleep(500000);
    resnum = rand() % 20;
    *count+=1;
    rptr->pids[*count] = getpid();
    rptr->req[*count][resnum] = instance;
    printf("\n Count: %d\n",*count);
    printf("\nExiting critical section\n");
    sem_signal(SemID);
    shmdt(shmid);
    return 0;
}

// Wait Function
void sem_wait(int semid) {
	struct sembuf sbuf;			
	sbuf.sem_num = 0;							
	sbuf.sem_op = -1;							
	sbuf.sem_flg = 0;							
	if (semop(semid, &sbuf, 1) == -1)  {
			exit(0);
	}
	return;
}

// Singal Function
void sem_signal(int semid) {
	struct sembuf sbuf;						
	sbuf.sem_num = 0;							
	sbuf.sem_op = 1;						
	sbuf.sem_flg = 0;						
	if (semop(semid, &sbuf, 1) == -1)  {
			exit(1);
	}
	return;
}
