/*********************************/
/*********************************/
/*Author: Manoj Ponagandla *******/
/*Title:  Resource Management*****/
/*Description : This is a simulation
to the actual Operating system 
Resource Management and Deadlocks*/
/*********************************/



#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>    
#include<sys/sem.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>
#include "data.h"
#include "config.h"
#include "deadlockmj.h"

#define key "/mysem1212"
#define shmkey 0x91919191
#define clockkey 0x41414141
#define reskey 0x11225566



int clockid;
int resid;
struct resDescriptor *rptr;
struct Clock *clk;
struct PCB *pcb;
int clockid,resid;
int SemID;
int maxlimit = 18;
int parallel_limit = 5;
int proc_parallel = 0;
int total_procs = 0;
int cflag = 1;

int *finish;
void semInit();
int main(int * argc, char * argv[])
{
    int i,j,count=0;
    pid_t pid;
    int status;
    int *scount;
    int shmid;
    time_t t;
    int rms;

    int req[18][20];
    fptr = fopen("log.txt","w");
    if(fptr == -1)
        perror("\nFile Open Error");
    //fprintf(fptr,"\nFile Opened");
    int SemKey = ftok(key,43);
    shmid = shmget(shmkey,sizeof(int),IPC_CREAT | 0777);
    if(shmid == -1)
    {
        perror("\n SHM Error");
    }
    scount = shmat(shmid,0,0); 
    *scount = 0;

    //Clock - Shared Memory
    if((clockid = shmget(clockkey, sizeof(struct Clock), IPC_CREAT | 0777)) == -1)
    {
        perror("\nClock creation error\n");
    
    }
    clk = shmat(clockid,0,0);
     clk->sec = 0;
	clk -> msec = 0;
    if((resid = shmget(reskey, sizeof(struct resDescriptor), IPC_CREAT | 0777)) == -1)
    {
        perror("\n Resource Descriptor not created\n");
    }
    rptr = shmat(resid,0,0);
    initRD();

    if ((SemID = semget(SemKey, 1, 0600 | IPC_CREAT)) == -1) {
		perror("Semaphore Error");
        exit(1);
    }
    
    srand((unsigned) time(&t));
    semInit();
    initClock();
    //initPCB();
    initAvailRes();
    while(cflag)
    {
        rms = (rand() % 501);
        //sem_wait(SemID);
        //advanceClock(rms);
       // sem_signal(SemID);
        if(total_procs <= maxlimit && proc_parallel < parallel_limit){
            pid = fork();
            proc_parallel++;
        }
        if(pid < 0){
            perror("\nFork error\n");
        }
        else if(pid == 0){
            execlp("./child","./child",(char *)NULL);
            exit(0);
        }
        
        
        count++;
        if(pid > 0){
            //printf("\nTotal number of processes forked are %d\n",*scount);
            pids[count] = getpid();
            printf("\n Children before exit: %d\n",count);
            printf("\nProc_parallel: %d\n",proc_parallel);
            if(count >= maxlimit)
            {
                printf("\nCount Exceeded\n");
                cflag = 0;
            }
            waitpid(pid, &status,0);
            proc_parallel--;
           
            printf("\n Children after exit: %d\n",count);

            
            //printf("\nI am parent and my PID %d",getppid());
        }

        //printf("\n Random number is: %d\n",rms);

        //printf("\nCurrent time is %d:%d",clk->sec,clk->msec);
        printf("\nMaster is running deadlock Algorithm");
        fprintf(fptr,"\nMaster is running deadlock algorithm\n");
        copySHMData(rptr->available,rptr->req,rptr->alloc,rptr->pids);
        //sem_wait(SemID);
        if(!(finish = deadlockmj(available_,20,18,request_,allocated_,pids_)))
        {
            printf("\n OSS: Master is suspending the process %d", getpid());
            fprintf(fptr,"\n OSS: Master is suspending the process %d", getpid());
        }
        //sem_signal(SemID);
        
    }

    for(i=0;i<20;i++)
    {
        printf("    R%i",i);
        //fprintf(fptr,"R%02i",i);
    }
    for(i=0;i<18;i++)
    {
        printf("\n");
        for(j=0;j<20;j++)
        {
            req[i][j] = rptr->req[i][j]; 
            printf("%4d",req[i][j]);
            //fprintf(fptr,"%4d",rptr->req[i][j]);
        }
    }


    printf("\n---Req Matrix---\n");

    for(i=0;i<20;i++)
        fprintf(fptr,"  R%0i",i);
    for(i=0;i<18;i++)
    {
        fprintf(fptr,"\n");
        for(j=0;j<20;j++)
        {
            fprintf(fptr," %4i",req[i][j]);
            //fprintf(fptr,"%4d",rptr->req[i][j]);
        }
    }

    printf("\n");
    for(i=0;i<20;i++)
        printf("%d ",rptr->available[i]);
    if ((semctl(SemID, 0, IPC_RMID, 1) == -1) && (errno != EINTR)) {
        perror("Semaphore : Removed");
    }
	shmdt(shmid);
    shmdt(clockid);
    shmdt(resid);
	if((shmctl(shmid, IPC_RMID, NULL)) == -1)
	{
	   perror("\n SHM Detach Error\n");	
	} else printf("\nSHM Detached\n");
    
    if((shmctl(resid, IPC_RMID, NULL)) == -1)
	{
	   perror("\n Resource Desc Detach Error\n");	
	} else printf("\nResource Desc Detached\n");
    
    if((shmctl(clockid, IPC_RMID, NULL)) == -1)
	{
	   perror("\n Clock Detach Error\n");	
	} else printf("\nClock Detached\n");
//sem_unlink("ponagand.");
return 0;
}

void semInit() {
    union semun { int val; struct semid_ds *buf; ushort * array; } argument;
    argument.val = 1;                            // Set the semaphore value to one
    if (semctl(SemID, 0, SETVAL, argument) == -1) {
       perror("oss: semctl (clock)");
        exit(1);
    }
    printf("Initialized Semaphore to 1");

    sem_wait(SemID);                       
    printf("\nIn Sem_Wait\n");
    sem_signal(SemID);                     
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

// Signal Function
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

void tandk()
{
    int i;
    printf("\nTerminating remaining processes");
    for(i= 0;i<maxlimit;i++)
    {
        kill(pids[i], SIGTERM);
    }
}

void initClock()
{
	clk -> sec = 0;
	clk -> msec = 0;
	//writedebug("\nClock initiated to 0s");
}

void advanceClock(int msec)
{
	clk -> msec += msec;
	if(clk -> msec > msec)
		{
			clk -> sec += clk -> msec/msec;
			clk -> msec %= msec;
		}	
}

void initRD()
{
    int i,j;
    for(i=0;i<18;i++)
    {
        rptr->pids[i] = 0;
        rptr->res_req[i] = 0;
        rptr->available[i] = 0;
        for(j=0;j<20;j++)
        {
            rptr->alloc[i][j] = 0;
            rptr->req[i][j] = 0;
        }
    
    }
}

void copySHMData(int *available,int **request, int **allocated, int *pids)
{
    int i,j;

    for(i=0;i<18;i++)
    {
        pids_[i] = rptr->pids[i];
        for(j=0;j<20;j++)
        {
            available_[j] = rptr->available[j];
            request_[i][j] = rptr->req[i][j];
            //printf("\nR: %d",rptr->req[i][j]);
            allocated_[i][j] = rptr->alloc[i][j];
        }
    }
    for(i=0;i<18;i++)
    {
        printf("\n");
        for(j=0;j<20;j++)
        {
            request_[i][j] = rptr->req[i][j]; 
            printf("%4d",request_[i][j]);
            //fprintf(fptr,"%4d",rptr->req[i][j]);
        }
    }
    printf("\n");

}
void initAvailRes()
{
    int i,r;
    for(i=0;i<20;i++)
    {
        usleep(300000);
        r= rand() % 6;
        rptr->available[i] = r;
    }
}